/*
 * A030401  Write 2n-1 in base 4 and juxtapose.
 * https://oeis.org/A030401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th digit of the concatenation of the base‑4
 *            representations of the odd numbers 1,3,5,7,…
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow4_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4_int.h"

seq_t a(idx_t n)
{
    unsigned __int128 pos = n; /* 1‑based position in the digit stream */
    unsigned int len = 1;
    /* Find the digit length group that contains position pos */
    for (;; ++len) {
        unsigned __int128 count_len;
        if (len == 1)
            count_len = 2;                             /* {1,3} */
        else
            count_len = ((unsigned __int128)3 * pow4_int(len - 1)) / 2; /* odd numbers of this length */
        unsigned __int128 digits_len = count_len * len;
        if (pos > digits_len)
            pos -= digits_len;
        else
            break;
    }

    /* Now pos is 1‑based index within the block of odd numbers of length len */
    unsigned __int128 zero_based = pos - 1;
    unsigned __int128 ix = zero_based / len;            /* which odd number in the block */
    unsigned int  digit_index = (unsigned int)(zero_based % len); /* position inside that number */

    unsigned __int128 low = pow4_int(len - 1);           /* first number of this block (4^{len‑1}) */
    unsigned __int128 m;
    if (len == 1)
        m = (unsigned __int128)1 + (unsigned __int128)2 * ix; /* 1,3 */
    else
        m = low + 1 + (unsigned __int128)2 * ix;            /* first odd is low+1, then +2 */

    unsigned __int128 pow_factor = pow4_int(len - 1 - digit_index);
    unsigned int digit = (unsigned int)((m / pow_factor) % 4);
    return (seq_t)digit;
}
