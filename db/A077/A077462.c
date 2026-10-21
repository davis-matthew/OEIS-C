/*
 * A077462  Prime factor configuration patterns.
 * https://oeis.org/A077462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9949, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A077462 – Prime factor configuration patterns
    a(n) is the rank of the exponent vector of n in its prime factorisation
    (1 contributes the empty vector).  The first number with a new vector
    receives a new rank equal to the largest previous rank plus one.
    a(0) = 0.                                                 */

#include <stdlib.h>
#include <stdint.h>

/*  static state  */
static seq_t *a_cache   = NULL;   /* cache of a(n) values          */
static idx_t  a_cache_sz  = 0;    /* number of computed entries    */
static idx_t  a_cache_cap = 0;    /* allocated capacity for cache  */

static struct Pattern {
    size_t len;              /* length of exponent vector      */
    uint8_t exps[16];        /* exponents in increasing prime */
} *patterns = NULL;          /* list of distinct patterns      */
static size_t pattern_cnt = 0; /* number of stored patterns      */
static size_t pattern_cap = 0;

/*  smallest‑prime‑factor array  */
static uint32_t *spf      = NULL;  /* spf[0..limit]                  */
static idx_t    spf_cap   = 0;    /* current size of spf array       */

/* ------------------------------------------------------------------ */
/*  resize a_cache  (C++ requires explicit cast)  */
static void enlarge_cache(idx_t min_cap)
{
    if (min_cap <= a_cache_cap) return;
    a_cache = (seq_t *)realloc(a_cache, min_cap * sizeof(seq_t));
    a_cache_cap = min_cap;
}

/* ------------------------------------------------------------------ */
/*  resize patterns array  */
static void enlarge_patterns(size_t min_cap)
{
    if (min_cap <= pattern_cap) return;
    patterns = (struct Pattern *)realloc(patterns,
                                         min_cap * sizeof(struct Pattern));
    pattern_cap = min_cap;
}

/* ------------------------------------------------------------------ */
/*  build spf array up to 'limit'  */
static void build_spf(idx_t limit)
{
    if (limit <= spf_cap) return;          /* already built */

    /*  Re‑allocate to the needed size  */
    free(spf);                         /* free old if any   */
    spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                  /* i is prime  */
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = limit;
}

/* ------------------------------------------------------------------ */
/*  linear search for a pattern; return 0 if not found  */
static seq_t find_pattern_rank(const uint8_t *vector, size_t len)
{
    for (size_t p = 0; p < pattern_cnt; ++p) {
        if (patterns[p].len != len) continue;
        const uint8_t *src = patterns[p].exps;
        int eq = 1;
        for (size_t i = 0; i < len; ++i)
            if (src[i] != vector[i]) { eq = 0; break; }
        if (eq) return (seq_t)(p + 1);   /* ranks start at 1 */
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/*  add a new pattern; return its new rank  */
static seq_t add_pattern(const uint8_t *vector, size_t len)
{
    enlarge_patterns(pattern_cnt + 1);
    patterns[pattern_cnt].len = len;
    for (size_t i = 0; i < len; ++i) patterns[pattern_cnt].exps[i] = vector[i];
    ++pattern_cnt;
    return (seq_t)pattern_cnt;          /* new rank */
}

/* ------------------------------------------------------------------ */
/*  compute a(n) up to n (inclusive)  */
static void compute_up_to(idx_t n)
{
    if (n < a_cache_sz) return;   /* already computed */

    enlarge_cache(n + 1);          /* ensure capacity */
    build_spf(n);                  /* spf up to n        */

    for (idx_t i = a_cache_sz; i <= n; ++i) {
        if (i == 0) {              /* a(0) = 0          */
            a_cache[0] = 0;
            continue;
        }

        /*  factorise i, collect exponents  */
        idx_t x = i;
        uint8_t exps_arr[16];
        size_t exps_len = 0;

        while (x > 1) {
            uint32_t p = spf[x];
            uint8_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            exps_arr[exps_len++] = cnt;
        }

        /*  lookup / assign rank  */
        seq_t rank = find_pattern_rank(exps_arr, exps_len);
        if (rank == 0) rank = add_pattern(exps_arr, exps_len);
        a_cache[i] = rank;
    }
    a_cache_sz = n + 1;
}

/* ------------------------------------------------------------------ */
/*  public function  */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}
