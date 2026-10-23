/*
 * A170300  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(k) = 3 * 2^(k-1) for 1 ≤ k ≤ 41 a(n) = a(n-1) +
 *            a(n-2) + … + a(n-42) for n ≥ 42 (equivalently a(n) = 2*a(n-1) –
 *            a(n-42)) The implementation uses the latter recurrence to avoid
 *            large intermediate sums
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n = 0 special case */
    if (n == 0)
        return (seq_t)1;

    /* n up to 41: closed‑form */
    if (n <= 41)
    {
        /* use a 64‑bit shift (1ULL) to stay in defined range, then cast to seq_t */
        return ((seq_t)(1ULL << (n - 1))) * (seq_t)3;
    }

    /* allocate a circular buffer of size 42 to hold the last 42 values */
    seq_t *buf = (seq_t *)malloc(42 * sizeof(seq_t));
    if (!buf)   /* sanity check – should never happen in test harness */
        return (seq_t)0;

    /* initialise buf[0..41] with a(0) … a(41) */
    buf[0] = (seq_t)1;                      /* a(0) */
    for (idx_t k = 1; k <= 41; ++k)
    {
        /* a(k) = 3 * 2^(k-1)  */
        buf[k] = ((seq_t)(1ULL << (k - 1))) * (seq_t)3;
    }

    /* iterate from n = 42 up to the requested index */
    for (idx_t i = 42; i <= n; ++i)
    {
        /* a(i) = 2 * a(i-1) – a(i-42) */
        seq_t prev   = buf[(i - 1) % 42];
        seq_t older  = buf[(i - 42) % 42];
        seq_t diff   = prev - older;        /* safe – guaranteed smaller than prev */
        seq_t next   = prev + diff;         /* equals 2*prev – older */

        buf[i % 42] = next;

        if (i == n)
        {
            free(buf);
            return next;
        }
    }

    /* unreachable path – to silence compiler warnings */
    free(buf);
    return (seq_t)0;
}
