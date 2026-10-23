/*
 * A101654  a(n)=Product{k=0..n, 1+4^A010060(k)}/2.
 * https://oeis.org/A101654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} (1 + 4^{A010060(k)}) / 2 A010060(k) is
 *            the Thue‑Morse sequence, i.e., parity of the number of 1-bits in
 *            the binary expansion of k
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   thue_morse_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/thue_morse_parity.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        prod *= (thue_morse_parity(k) == 0) ? (seq_t)2 : (seq_t)5;
    }
    prod /= 2;   /* the product is always even because k=0 contributes 2 */
    return prod;
}
