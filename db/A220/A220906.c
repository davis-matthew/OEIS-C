/*
 * A220906  Thue-Morse sequence (A010060) with 0 replaced by 2 and 1 replaced
 *          by 3,1.
 * https://oeis.org/A220906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by expanding the Thue–Morse sequence with the
 *            map 0 → 2, 1 → 3,1. Each 0 contributes a single value 2; each 1
 *            contributes two values, the first being 3 and the second 1
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds the original Thue–Morse index that contains
   the n‑th expanded term and returns the corresponding value. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i = 0;          /* original Thue–Morse index */
    idx_t sum = 0;        /* total number of expanded terms processed */

    while (1)
    {
        /* compute parity of i: 0 → even, 1 → odd */
        idx_t x = i;
        int parity = 0;
        while (x)
        {
            parity ^= (int)(x & 1);
            x >>= 1;
        }

        /* weight of the current original symbol:
           1 if it is 0, 2 if it is 1        */
        idx_t weight = (idx_t)1 + (idx_t)parity;

        /* stop when the n‑th expanded term falls inside this block */
        if (sum + weight > n)
            break;

        sum += weight;
        ++i;
    }

    /* Determine the value at position n within the block of a(i) */
    idx_t x = i;
    int parity = 0;
    while (x)
    {
        parity ^= (int)(x & 1);
        x >>= 1;
    }

    if (parity == 0)
        return (seq_t)2;                 /* 0 → 2 */
    else if (n - sum == 0)
        return (seq_t)3;                 /* first part of 1 → 3,1 */
    else
        return (seq_t)1;                 /* second part of 1 → 3,1 */
}
