/*
 * A188773  Number of nX8 binary arrays without the pattern 0 1 0 vertically
 *          or horizontally.
 * https://oeis.org/A188773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188773 – number of n×8 binary matrices with no 010
    horizontally or vertically.
    Implementation builds the 114 allowed 8‑bit rows, precomputes
    all admissible next rows for each consecutive pair, and
    uses dynamic programming over the last two rows.  The cache
    allows calls in any order.  The code compiles in both C11
    and C++17 – in C++ arbitrary‑precision seq_t objects are
    allocated with new, in C ordinary malloc/calloc are used.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* function signature (see problem statement) */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*                         global data                                 */
/* ------------------------------------------------------------------ */
static size_t m = 0;          /* number of allowed 8‑bit rows            */
static size_t mm = 0;         /* m*m – number of row pairs               */

static uint8_t *rows = NULL;  /* list of allowed rows                   */

struct Trans {
    int   *c;   /* array of compatible next row indices  */
    size_t sz;  /* current number of entries               */
    size_t cap; /* allocated capacity                      */
};
static struct Trans *trans = NULL;        /* array of size mm                    */

static seq_t *dp_cur = NULL;              /* DP for current length (>=2)           */
static seq_t *dp_next = NULL;             /* DP for next length                    */

static seq_t *cache = NULL;               /* cache of a(1..last_computed)          */
static idx_t   cache_cap = 0;             /* allocated length of cache array       */
static idx_t   cache_last = 0;             /* largest n whose value is cached        */

/* ------------------------------------------------------------------ */
/*                build the list of admissible 8‑bit rows              */
/* ------------------------------------------------------------------ */
static void build_rows(void)
{
    uint8_t tmp[256];
    size_t sz = 0;
    for (int val = 0; val < 256; ++val) {
        bool ok = true;
        for (int i = 0; i < 6; ++i) {
            int b2 = (val >> (i + 2)) & 1;
            int b1 = (val >> (i + 1)) & 1;
            int b0 = (val >>  i       ) & 1;
            if (b2 == 0 && b1 == 1 && b0 == 0) { ok = false; break; }
        }
        if (ok) tmp[sz++] = (uint8_t)val;
    }
    rows = (uint8_t*)malloc(sz * sizeof(uint8_t));
    memcpy(rows, tmp, sz);
    m = sz;
}

/* ------------------------------------------------------------------ */
/*               build transitions for each consecutive pair           */
/* ------------------------------------------------------------------ */
static void build_transitions(void)
{
    mm = m * m;
    trans = (struct Trans*)malloc(mm * sizeof(struct Trans));

    for (size_t a = 0; a < m; ++a) {
        for (size_t b = 0; b < m; ++b) {
            size_t idx = a * m + b;
            trans[idx].cap = (size_t)m;
            trans[idx].sz  = 0;
            trans[idx].c   = (int*)malloc(m * sizeof(int));

            for (size_t c = 0; c < m; ++c) {
                bool ok = true;
                for (int col = 0; col < 8; ++col) {
                    int a_bit = (rows[a] >> col) & 1;
                    int b_bit = (rows[b] >> col) & 1;
                    int c_bit = (rows[c] >> col) & 1;
                    if (a_bit == 0 && b_bit == 1 && c_bit == 0) { ok = false; break; }
                }
                if (ok) trans[idx].c[trans[idx].sz++] = (int)c;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*                       lazy initialisation                           */
/* ------------------------------------------------------------------ */
static void init_state(void)
{
    if (m != 0) return;          /* already initialised */

    build_rows();
    build_transitions();

    /* DP arrays for length >= 2 */
#ifdef __cplusplus
    dp_cur  = new seq_t[mm];
    dp_next = new seq_t[mm];
    for (size_t i = 0; i < mm; ++i) dp_cur[i] = 0;
    for (size_t i = 0; i < mm; ++i) dp_next[i] = 0;
#else
    dp_cur  = (seq_t*)calloc(mm, sizeof(seq_t));
    dp_next = (seq_t*)calloc(mm, sizeof(seq_t));
#endif

    /* DP for length 2 – all pairs equally possible */
    for (size_t i = 0; i < mm; ++i) dp_cur[i] = 1;

    /* cache for a(1) and a(2) */
    cache_cap = 2;
#ifdef __cplusplus
    cache = new seq_t[cache_cap + 1];
#else
    cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
#endif
    cache[0] = 0;
    cache[1] = (seq_t)m;
    cache[2] = (seq_t)m * (seq_t)m;
    cache_last = 2;
}

/* ------------------------------------------------------------------ */
/*                      enlarge cache array if required                 */
/* ------------------------------------------------------------------ */
static void grow_cache(idx_t n)
{
    if (cache_cap >= (size_t)n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < (size_t)n) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap + 1];
    for (size_t i = 0; i <= cache_cap; ++i) new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    seq_t *new_cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*                            main routine                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety for out of bounds */

    init_state();

    if (n <= cache_last) return cache[n];

    grow_cache(n);

    /* compute values from cache_last+1 up to n */
    for (idx_t len = cache_last + 1; len <= n; ++len) {
        /* reset dp_next to zero */
#ifdef __cplusplus
        for (size_t i = 0; i < mm; ++i) dp_next[i] = 0;
#else
        memset(dp_next, 0, mm * sizeof(seq_t));
#endif

        for (size_t p = 0; p < mm; ++p) {
            seq_t val = dp_cur[p];
            if (val == 0) continue;
            size_t b   = p % m;
            struct Trans *t = &trans[p];
            for (size_t k = 0; k < t->sz; ++k) {
                size_t c   = t->c[k];
                size_t idx = b * m + c;
                dp_next[idx] += val;
            }
        }

        /* sum all sequences of this length */
        seq_t total = 0;
        for (size_t i = 0; i < mm; ++i) total += dp_next[i];
        cache[len] = total;

        /* prepare for next length */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cache_last = n;
    return cache[n];
}
