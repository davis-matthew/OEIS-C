/*
 * A057575  Maximal numbers of codewords in mixed code with n binary
 *          coordinates and 2 ternary coordinates with Hamming distance 3.
 * https://oeis.org/A057575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 9 * 2^(n-4) for n >= 7, with small values a(0..6) as
 *            listed
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* small known values */
    static const seq_t small[7] = {
        (seq_t)1,  /* a(0) */
        (seq_t)2,  /* a(1) */
        (seq_t)4,  /* a(2) */
        (seq_t)6,  /* a(3) */
        (seq_t)12, /* a(4) */
        (seq_t)22, /* a(5) */
        (seq_t)38  /* a(6) */
    };

    if (n < 7)
        return small[n];

    /* For n >= 7, a(n) = 9 * 2^(n-4) */
    return ((seq_t)9) << (n - 4);
}
