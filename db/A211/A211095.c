/*
 * A211095  Length of the smallest (i.e., rightmost) Lyndon word in the Lyndon
 *          factorization of the binary representation of n.
 * https://oeis.org/A211095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   length of the rightmost Lyndon factor in the Lyndon
 *            factorization of the binary representation of n
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The binary representation of 0 is just '0'. */
    if (n == 0)
        return (seq_t)1;

    /* Determine the number of bits in n. */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* Store the bits most‑significant first. */
    unsigned char bits[64];
    for (size_t i = 0; i < len; ++i)
        bits[i] = (n >> (len - 1 - i)) & 1u;

    /* Duval's algorithm for Lyndon factorization.
       We keep track of the length of the last factor produced. */
    size_t i = 0;
    size_t last_len = 0;

    while (i < len) {
        size_t j = i + 1;
        size_t k = i;
        while (j < len && bits[k] <= bits[j]) {
            if (bits[k] < bits[j])
                k = i;
            else
                ++k;
            ++j;
        }
        size_t l = j - k;          /* length of the next Lyndon factor */
        while (i <= k) {           /* repeat if necessary */
            last_len = l;
            i += l;
        }
    }

    return (seq_t)last_len;
}
