/*
 * A168055  Expansion of 2 - x - sqrt(1-2x-3x^2).
 * https://oeis.org/A168055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(0)=1, a(1)=0, a(2)=2. * For n≥3: a(n) = ((2n−3)*a(n−1) +
 *            3(n−3)*a(n−2)) / n * (this follows from the D‑finite recurrence
 *            * n*a(n)+(-2n+3)*a(n−1)+3(-n+3)*a(n−2)=0 ) * * The computation
 *            uses a cache so calls in any order run in * O(max n) time and
 *            O(max n) memory. The intermediate * products ar...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* table of computed values   */
static idx_t  last  = 0;      /* highest index that is defined */
static size_t cap   = 0;      /* current capacity of cache   */

#ifdef __cplusplus
/* C++ path – seq_t may be an arbitrary‑precision class */
static inline seq_t compute_next(idx_t i, seq_t a1, seq_t a2)
{
    return (a1 * (seq_t)(2 * i - 3) + a2 * (seq_t)(3 * (i - 3))) / (seq_t)i;
}
#else
/* C path – seq_t is a built‑in integer type; use 128‑bit intermediates */
static inline seq_t compute_next(idx_t i, seq_t a1, seq_t a2)
{
    unsigned __int128 num =
        (unsigned __int128)a1 * (2 * i - 3) +
        (unsigned __int128)a2 * (3 * (i - 3));
    return (seq_t)(num / i);
}
#endif

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* allocate minimal capacity if this is the first non‑base call */
    if (cache == NULL) {
        cap = 3;                     /* indices 0..2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        last = 2;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 4;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* realloc failed */
        cache = tmp;
        cap = new_cap;
    }

    /* if already computed, return immediately */
    if (n <= last) return cache[n];

    /* compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = compute_next(i, cache[i - 1], cache[i - 2]);
    }
    last = n;
    return cache[n];
}
