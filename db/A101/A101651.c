/*
 * A101651  a(n) = Product_{k=0..n} (1 + 0^A010060(k))/2.
 * https://oeis.org/A101651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^{A245710(n)}, where A245710(n) = number of integers k
 *            with 1 ≤ k ≤ n that have an even number of 1‑bits in their
 *            binary expansion
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   bitlen, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Count how many integers 1 ≤ k ≤ n have even popcount(k). */
    if (n == 0)                 /* a(0) = 1 = 2^0 */
        return pow2((idx_t)0);

    idx_t count = 0;            /* accumulate numbers counted (including 0 and n) */
    int parity = 0;             /* parity of the prefix already processed */
    int bits = bitlen(n);       /* number of bits needed to represent n */

    for (int i = bits - 1; i >= 0; --i) {
        if (n & ((idx_t)1 << i)) {      /* current bit in n is 1 */
            if (i == 0) {
                if (parity == 0)
                    ++count;            /* numbers where this bit is 0 and
                                           the suffix gives even parity */
            } else {
                /* 2^{i-1} numbers of length i with even parity */
                count += (idx_t)1 << (i - 1);
            }
            parity ^= 1;              /* we now set this bit to 1 (as in n) */
        }
    }

    /* Add n itself if it has even popcount. */
    if (parity == 0)
        ++count;

    /* Subtract the count for k = 0, which was included during the loop. */
    if (count > 0)
        --count;

    return pow2(count);
}
