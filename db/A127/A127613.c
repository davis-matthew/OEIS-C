/*
 * A127613  a(n) = denominator of the continued fraction which has the
 *          positive divisors of n as its terms. The terms are written in
 *          order from n for the integer part, to 1 for the final term of the
 *          continued fraction.
 * https://oeis.org/A127613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the continued fraction formed by the
 *            divisors of n, listed from n down to 1
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: integer square root of an unsigned 64‑bit integer.
   Returns ⌊√n⌋. Uses 128‑bit arithmetic to avoid overflow. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1, hi = n >> 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* Comparator for qsort: ascending order of idx_t. */
static int cmp_idx(const void *a, const void *b)
{
    idx_t ia = *(const idx_t *)a;
    idx_t ib = *(const idx_t *)b;
    return (ia < ib) - (ia > ib);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Maximum number of divisors for n < 2^64 is < 2000; use a static
       array that fits all test cases. */
    idx_t divs[4096];
    size_t count = 0;

    /* Enumerate all divisors of n. */
    idx_t sqrt_n = isqrt_u64(n);
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            divs[count++] = i;
            if (i != n / i) {
                divs[count++] = n / i;
            }
        }
    }

    /* Sort so that divs[0] = 1, divs[count-1] = n. */
    qsort(divs, count, sizeof(idx_t), cmp_idx);

    /* Evaluate the continued fraction from the largest divisor downwards. */
    seq_t p = (seq_t)divs[count - 1];   /* current numerator   */
    seq_t q = (seq_t)1;                 /* current denominator */

    for (size_t idx = count - 1; idx > 0; --idx) {
        seq_t d = (seq_t)divs[idx - 1];
        seq_t new_p = d * p + q;
        seq_t new_q = p;
        p = new_p;
        q = new_q;
    }

    return q; /* denominator of the continued fraction */
}
