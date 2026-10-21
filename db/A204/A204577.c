/*
 * A204577  Sqrt(floor[A204575(n)/2]), written in binary.
 * https://oeis.org/A204577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number whose decimal digits are exactly the *
 *            binary digits of Pell(2*n-4) (Pell numbers defined by * P(0)=0,
 *            P(1)=1, P(k)=2*P(k-1)+P(k-2)). a(1)=a(2)=0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"   /* unsigned bitlen_seq_t(seq_t x); */

seq_t a(idx_t n)
{
    /* offsets: a(1)=a(2)=0 */
    if (n < 3)
        return 0;

    /* 2*n - 4 fits in idx_t for n up to 10^18 */
    idx_t k = (idx_t)2 * n - 4;          /* Pell index */

    /* Compute Pell(k) iteratively */
    seq_t p0 = 0;
    seq_t p1 = 1;
    if (k == 0)
        return p0;
    if (k == 1)
        return p1;

    for (idx_t i = 2; i <= k; ++i) {
        seq_t p2 = p1 * 2 + p0;  /* p2 = 2*p1 + p0 */
        p0 = p1;
        p1 = p2;
    }
    seq_t pell_val = p1;          /* Pell(k) */

    /* Convert the binary representation of pell_val to a decimal
     * number whose digits are 0/1 only.
     */
    seq_t result = 0;
    unsigned bits = bitlen_seq_t(pell_val);
    for (int i = (int)bits - 1; i >= 0; --i) {
        seq_t bit = (pell_val >> i) & 1;
        result = result * 10 + bit;
    }
    return result;
}
