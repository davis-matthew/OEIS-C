/*
 * A052516  Number of pairs of sets of cardinality at least 3.
 * https://oeis.org/A052516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max(0, 2^n – n^2 – n – 2) (OEIS A052516)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special handling for 64‑bit builds */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        if (n > 64)                         /* values beyond 64 still fit, but
                                                the 64‑bit build is not required
                                                to compute them */
            return (seq_t)0;

        seq_t nseq = (seq_t)n;
        seq_t poly = nseq * nseq + nseq + (seq_t)2;   /* n^2 + n + 2 */

        if (n < 64)                         /* 2^n fits in 64 bits */
        {
            seq_t pow2 = ((seq_t)1) << n;    /* 2^n */
            return pow2 > poly ? pow2 - poly : (seq_t)0;
        }
        else                                /* n == 64 ; 2^64 does not fit in 64 bits */
        {
            /* 2^64 – c  =  UINT64_MAX – (c – 1)  for any 0 < c < 2^64  */
            seq_t res = (seq_t)UINT64_MAX - ((poly - (seq_t)1));
            return res;
        }
    }

    /* General case – e.g. arbitrary‑precision types that support
       left‑shift by large amounts */
    seq_t pow2 = ((seq_t)1) << n;          /* 2^n */
    seq_t nseq  = (seq_t)n;
    seq_t poly  = nseq * nseq + nseq + (seq_t)2;   /* n^2 + n + 2 */
    return pow2 > poly ? pow2 - poly : (seq_t)0;
}
