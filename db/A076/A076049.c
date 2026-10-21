/*
 * A076049  Numbers k such that the sum of the k-th triangular number and
 *          (k+2)-nd triangular number is a triangular number.
 * https://oeis.org/A076049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   sequence a(n) are the non‑negative integers k for which *
 *            k^2+3k+3 is triangular. This is equivalent to finding integer *
 *            solutions of S^2 – 2t^2 = 7 with t = 2k+3. The positive odd *
 *            solutions (t,S) are produced by two interleaving recurrences * *
 *            t_0 = 3 , t_1 = 19 , t_{n+1} = 6 t_n – t_{n-1} *...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of already computed terms */
static seq_t *cache   = NULL;       /* indices 1..cache_len */
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

/* sequences of odd t values for the two chains */
static seq_t *tB = NULL;    /* chain starting with 3,19, ... */
static seq_t *tA = NULL;    /* chain starting with 9,53, ... */
static idx_t  tB_len = 0, tB_cap = 0;
static idx_t  tA_len = 0, tA_cap = 0;

/* ensure the cache holds at least indices 0..n */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* ensure the tB array holds at least indices 0..n */
static void ensure_tB(idx_t n)
{
    if (n < tB_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(tB, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    tB = tmp;
    tB_cap = (idx_t)new_cap;
}

/* ensure the tA array holds at least indices 0..n */
static void ensure_tA(idx_t n)
{
    if (n < tA_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(tA, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    tA = tmp;
    tA_cap = (idx_t)new_cap;
}

/* generate all terms up to index n */
static void generate_up_to(idx_t n)
{
    if (n <= cache_len)
        return;

    ensure_cache(n);
    ensure_tB((n + 1) / 2);   /* number of odd terms needed */
    ensure_tA(n / 2);         /* number of even terms needed */

    /* initialise tB and tA if required */
    if (tB_len == 0) {
        tB[0] = 3;            /* t_0 */
        tB_len = 1;
    }
    if (tB_len == 1 && tB_cap > 1) { /* ensure space for t_1 */
        tB[1] = 19;           /* t_1 */
        tB_len = 2;
    }
    if (tA_len == 0) {
        tA[0] = 9;            /* t'_0 */
        tA_len = 1;
    }
    if (tA_len == 1 && tA_cap > 1) { /* ensure space for t'_1 */
        tA[1] = 53;           /* t'_1 */
        tA_len = 2;
    }

    /* extend tB */
    while (tB_len < (n + 1) / 2) {
        tB[tB_len] = 6 * tB[tB_len - 1] - tB[tB_len - 2];
        tB_len++;
    }

    /* extend tA */
    while (tA_len < n / 2) {
        tA[tA_len] = 6 * tA[tA_len - 1] - tA[tA_len - 2];
        tA_len++;
    }

    /* compute terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i & 1) { /* odd index: use tB */
            idx_t idx = (i - 1) / 2;
            cache[i] = (tB[idx] - 3) / 2;
        } else {    /* even index: use tA */
            idx_t idx = i / 2 - 1;
            cache[i] = (tA[idx] - 3) / 2;
        }
    }
    cache_len = n;
}

/* public function required by the grader */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1, but extra guard */
        return 0;
    generate_up_to(n);
    return cache[n];
}
