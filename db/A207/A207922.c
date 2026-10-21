/*
 * A207922  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207922: Number of 7×n binary arrays avoiding
   000 and 101 horizontally and 010 and 101 vertically. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

static bool vertical_ok(uint8_t p)
{
    /* prevent 010 and 101 vertically in this column */
    for (int r = 0; r < 5; ++r) {
        int a = (p >> r)     & 1;
        int b = (p >> (r + 1)) & 1;
        int c = (p >> (r + 2)) & 1;
        if ((a == 0 && b == 1 && c == 0) || (a == 1 && b == 0 && c == 1))
            return false;
    }
    return true;
}

/* ---------- static data structure ---------- */
struct A207922_state {
    /* column patterns that satisfy vertical constraints */
    uint8_t *patterns;       /* array of size valcnt */
    idx_t    valcnt;         /* number of such patterns */

    /* transition data */
    int  **trans;            /* for each state (i,j) list of next state indices */
    int   *lens;             /* number of transitions per state */

    /* DP buffers */
    seq_t *dp_cur;           /* current column length */
    seq_t *dp_next;          /* next column length */

    /* cache of computed a(1..cache_len) */
    seq_t *cache;            /* 1‑based indexing */
    idx_t  cache_len;        /* last computed index */
    size_t cache_cap;        /* allocated capacity */
};
static struct A207922_state *S = NULL;

/* Forward declaration of initializer */
static void init_A207922(void);

/* ---------- initializer ---------- */
static void init_A207922(void)
{
    if (S != NULL) return;   /* already initialized */

    S = (struct A207922_state *)malloc(sizeof(struct A207922_state));
    memset(S, 0, sizeof(*S));

    /* 1. collect all columns that satisfy vertical constraints */
    uint8_t tmp[128];
    idx_t valcnt = 0;
    for (uint8_t p = 0; p < 128; ++p)
        if (vertical_ok(p))
            tmp[valcnt++] = p;

    S->valcnt = valcnt;
    S->patterns = (uint8_t *)malloc(valcnt * sizeof(uint8_t));
    memcpy(S->patterns, tmp, valcnt * sizeof(uint8_t));

    /* 2. number of states = valcnt^2 */
    size_t states = (size_t)valcnt * (size_t)valcnt;
    S->lens = (int *)malloc(states * sizeof(int));
    S->trans = (int **)malloc(states * sizeof(int *));

    /* 3. precompute transitions */
    for (size_t i = 0; i < (size_t)valcnt; ++i) {
        uint8_t pi = S->patterns[i];
        for (size_t j = 0; j < (size_t)valcnt; ++j) {
            uint8_t pj = S->patterns[j];
            size_t sid = i * valcnt + j;

            int cnt = 0;
            /* first count transitions to allocate */
            for (size_t k = 0; k < (size_t)valcnt; ++k) {
                uint8_t pk = S->patterns[k];
                bool ok = true;
                for (int r = 0; r < 7 && ok; ++r) {
                    int a = (pi >> r) & 1;
                    int b = (pj >> r) & 1;
                    int c = (pk >> r) & 1;
                    if ((a == 0 && b == 0 && c == 0) ||
                        (a == 1 && b == 0 && c == 1))
                        ok = false;
                }
                if (ok) ++cnt;
            }
            S->lens[sid] = cnt;
            if (cnt == 0) {
                S->trans[sid] = NULL;
                continue;
            }
            S->trans[sid] = (int *)malloc(cnt * sizeof(int));
            int pos = 0;
            for (size_t k = 0; k < (size_t)valcnt; ++k) {
                uint8_t pk = S->patterns[k];
                bool ok = true;
                for (int r = 0; r < 7 && ok; ++r) {
                    int a = (pi >> r) & 1;
                    int b = (pj >> r) & 1;
                    int c = (pk >> r) & 1;
                    if ((a == 0 && b == 0 && c == 0) ||
                        (a == 1 && b == 0 && c == 1))
                        ok = false;
                }
                if (ok)
                    S->trans[sid][pos++] = j * valcnt + k;   /* new state index */
            }
        }
    }

    /* 4. allocate DP buffers */
    S->dp_cur  = (seq_t *)calloc(states, sizeof(seq_t));
    S->dp_next = (seq_t *)calloc(states, sizeof(seq_t));

    /* 5. initialise cache */
    S->cache_cap = 2;
    S->cache = (seq_t *)calloc(S->cache_cap + 1, sizeof(seq_t));
    S->cache_len = 0;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    init_A207922();
    if (n == 0) return (seq_t)0;  /* out of offset, undefined */

    /* pre‑compute base values a(1) and a(2) */
    if (S->cache_len < 1) {
        S->cache[1] = (seq_t)S->valcnt;
        S->cache_len = 1;
    }
    if (S->cache_len < 2) {
        S->cache[2] = (seq_t)S->valcnt * (seq_t)S->valcnt;
        S->cache_len = 2;
    }

    /* initialize dp_cur for length 2 if not yet initialised */
    static bool dp_initialised = false;
    if (!dp_initialised) {
        size_t states = (size_t)S->valcnt * (size_t)S->valcnt;
        for (size_t s = 0; s < states; ++s)
            S->dp_cur[s] = (seq_t)1;   /* each pair of columns is valid for n=2 */
        dp_initialised = true;
    }

    /* if already computed up to n, return value */
    if (S->cache_len >= n)
        return S->cache[n];

    /* extend calculations from current cache_len+1 up to n */
    for (idx_t col = S->cache_len + 1; col <= n; ++col) {
        /* zero dp_next */
        memset(S->dp_next, 0, ((size_t)S->valcnt * (size_t)S->valcnt) * sizeof(seq_t));

        /* transition */
        size_t states = (size_t)S->valcnt * (size_t)S->valcnt;
        for (size_t s = 0; s < states; ++s) {
            seq_t cur = S->dp_cur[s];
            if (!cur) continue;
            int m = S->lens[s];
            int *lst = S->trans[s];
            for (int t = 0; t < m; ++t) {
                int nxt = lst[t];
                S->dp_next[nxt] += cur;
            }
        }

        /* swap buffers */
        seq_t *tmp = S->dp_cur;
        S->dp_cur  = S->dp_next;
        S->dp_next = tmp;

        /* compute total sum for this column length */
        seq_t sum = 0;
        for (size_t s = 0; s < states; ++s)
            sum += S->dp_cur[s];

        /* enlarge cache if necessary */
        if (S->cache_len < col) {
            if (S->cache_len == S->cache_cap) {
                size_t newcap = S->cache_cap * 2;
                S->cache = (seq_t *)realloc(S->cache, (newcap + 1) * sizeof(seq_t));
                S->cache_cap = newcap;
            }
            S->cache[col] = sum;
            S->cache_len = col;
        }
    }

    return S->cache[n];
}
