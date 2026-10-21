/*
 * A084483  If the rightmost block of zeros in binary representation of n has
 *          an even length, then delete one 0, otherwise insert one 0 in this
 *          block.
 * https://oeis.org/A084483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) transforms n by modifying the right‑most block of zeros: if
 *            that block has even length, delete one 0; else insert one 0. The
 *            function implements this rule for all n ≥ 1 and is a
 *            self‑inverse permutation
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for n==0, though unused */

    idx_t t   = n;                      /* copy for manipulation   */
    idx_t k   = 0;                      /* number of trailing 1s   */

    /* count trailing ones */
    while ((t & 1UL) == 1UL) {
        t >>= 1;
        k++;
    }

    /* if no zero exists (all ones), return n unchanged */
    if (t == 0) return (seq_t)n;

    idx_t len = 0;                      /* length of trailing zero block */

    /* count consecutive zeros following the trailing ones */
    while ((t & 1UL) == 0UL) {
        t >>= 1;
        len++;
    }

    idx_t H = t;                        /* higher bits above the zero block */

    /* adjust the zero block length: delete if even, insert if odd */
    idx_t len2 = (len & 1UL) ? (len + 1ULL) : (len - 1ULL);

    /* assemble the result */
    seq_t res = ((seq_t)H) << (len2 + k);
    res += (((seq_t)1ULL) << k) - 1ULL;

    return res;
}
