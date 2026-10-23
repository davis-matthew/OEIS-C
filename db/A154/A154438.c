/*
 * A154438  Permutation of nonnegative integers: A059893-conjugate of A154436.
 * https://oeis.org/A154438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0, a(1) = 1.
   For m > 0 and 0 <= k < 2^m:
     a(2^(m+2)-1-2*k) = 2 * a(2^m + k)
     a(2^(m+1)+2*k)   = 2 * a(2^m + k) + 1
   (The recurrence holds also for m = 0.) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)             /* base cases */
        return (seq_t)n;

    /* compute floor(log2(n)) */
    idx_t t = 0, tmp = n;
    while ((tmp >>= 1))
        ++t;

    idx_t m   = t - 1;                /* m >= 0 */
    idx_t pow_m      = (idx_t)1 << m;  /* 2^m */
    idx_t pow_m_plus1= pow_m << 1;    /* 2^(m+1) */

    if ((n & (idx_t)1) == 0)          /* even case: a(2^(m+1)+2*k) */
    {
        idx_t k = (n - pow_m_plus1) >> 1;
        return (seq_t)2 * a(pow_m + k) + 1;
    }
    else                              /* odd case: a(2^(m+2)-1-2*k) */
    {
        /* mask for 2^(m+2)-1, avoiding overflow when m+2==64 */
        idx_t mask_even;
        if (m + 2 < 64)
            mask_even = ((idx_t)1 << (m + 2)) - 1;
        else
            mask_even = (idx_t)-1;    /* UINT64_MAX */

        idx_t diff = (~n) & mask_even;
        idx_t k    = (diff >> 1) & (pow_m - 1);
        return (seq_t)2 * a(pow_m + k);
    }
}
