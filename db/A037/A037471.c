/*
 * A037471  a(n)=Sum{d(i)*8^i: i=0,1,...,m}, where Sum{d(i)*6^i: i=0,1,...,m}
 *          is the base 6 representation of n.
 * https://oeis.org/A037471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{d(i) * 8^i : digits d(i) of n in base 6}
 * Range:     u8 n <= 143, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    idx_t x = n;
    while (x != 0) {
        idx_t digit = x % 6;            /* base‑6 digit (0…5) */
        result += (seq_t)digit * power; /* add digit * 8^i to result */
        power *= 8;                     /* move to next power of 8 */
        x /= 6;                         /* next base‑6 digit */
    }
    return result;
}
