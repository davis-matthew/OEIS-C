/*
 * A222235  In the number n, replace all (decimal) digits '3' with '5' and
 *          vice versa.
 * https://oeis.org/A222235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) by swapping decimal digits 3 and 5 of n in base‑10
 * Range:     u8 n <= 235, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t place  = 1;
    while (n > 0) {
        unsigned d = (unsigned)(n % 10);
        if (d == 3) d = 5;
        else if (d == 5) d = 3;
        result += (seq_t)d * place;
        place *= 10;
        n /= 10;
    }
    return result;
}
