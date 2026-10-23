/*
 * A037456  a(n)=Sum{d(i)*8^i: i=0,1,...,m}, where Sum{d(i)*3^i: i=0,1,...,m}
 *          is the base 3 representation of n.
 * https://oeis.org/A037456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{ d(i) * 8^i : i = 0,1,...,m } where Σ d(i) * 3^i is
 *            the base‑3 representation of n
 * Range:     u8 n <= 26, u16 n <= 485, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    idx_t temp   = n;

    while (temp != 0) {
        unsigned digit = (unsigned)(temp % 3);
        result += (seq_t)digit * power;  /* add digit * 8^i */
        power *= 8;                    /* advance to next power of 8 */
        temp /= 3;
    }

    return result;
}
