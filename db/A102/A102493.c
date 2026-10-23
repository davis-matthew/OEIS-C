/*
 * A102493  Numbers in base-60 representation that can be written with decimal
 *          digits.
 * https://oeis.org/A102493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{i≥0} d_i * 60^i, where d_i are the decimal digits of
 *            (n−1)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; guard against accidental 0 calls. */
    if (n == 0) return 0;

    idx_t t = n - 1;          /* decimal number to decompose */
    seq_t res   = 0;          /* accumulating value in base‑60 */
    seq_t power = 1;          /* 60^0 initially */

    while (t > 0) {
        unsigned digit = (unsigned)(t % 10);   /* least‑significant decimal digit */
        res += (seq_t)digit * power;
        t /= 10;
        power *= (seq_t)60;                  /* next base‑60 power */
    }

    return res;
}
