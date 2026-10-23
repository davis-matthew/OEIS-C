/*
 * A008935  If 2n = Sum 2^e(k) then a(n) = Sum e(k)^2.
 * https://oeis.org/A008935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = sum_{k} (e_k + 1)^2 where e_k are positions of 1‑bits in
 *            binary representation of n (i.e. for 2n = sum 2^{e_k}, a(n)=sum
 *            e_k^2). This is equivalent to summing the squares of
 *            (position+1) of each set bit in n because 2n = n << 1
 * Range:     u8 n <= 492, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any idx_t>=1; for n=0 it returns 0. */

#include <stdint.h> /* for uint64_t if needed */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n;
    idx_t pos = 0;

    /* Process each bit of n; pos is the exponent (0‑based) of the current bit. */
    while (m) {
        if (m & 1) {
            seq_t e = (seq_t)(pos + 1);   /* exponent in 2n is pos+1 */
            sum += e * e;
        }
        m >>= 1;
        ++pos;
    }

    return sum;
}
