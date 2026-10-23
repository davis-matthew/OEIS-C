/*
 * A120196  a(1)=3; a(n) = floor((26 + Sum_{j=1..n-1} a(j))/8).
 * https://oeis.org/A120196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; a(n)=floor((26+Σ_{j=1}^{n-1} a(j))/8)
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoization state */
static seq_t *cache = NULL;
static seq_t *pref  = NULL;   /* pref[i] = Σ_{j=1}^{i+1} a(j) */
static idx_t len    = 0;
static idx_t cap    = 0;

/* Ensure enough capacity for at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cap)
        return;
    idx_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *c = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!c) abort();
    seq_t *p = (seq_t*)realloc(pref, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache = c;
    pref  = p;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, return 0 */

    if (len >= n)
        return cache[n - 1];

    ensure_capacity(n);

    if (len == 0) {
        /* Initialize first term a(1) = 3 */
        cache[0] = (seq_t)3;
        pref[0]  = cache[0];
        len = 1;
    }

    for (idx_t i = len + 1; i <= n; ++i) {
        /* Sum of a(1)..a(i-1) is pref[i-2] */
        seq_t sum = pref[i - 2];
        seq_t val = (sum + (seq_t)26) / (seq_t)8;   /* floor division */
        cache[i - 1] = val;
        pref[i - 1]  = pref[i - 2] + val;
    }
    len = n;
    return cache[n - 1];
}
