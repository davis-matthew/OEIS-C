/*
 * A100990  a(n) = n^21 mod 100.
 * https://oeis.org/A100990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^21 mod 100
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)(n % 100);   /* reduce modulo 100 */
    seq_t r = 1;
    unsigned e = 21;
    while (e) {
        if (e & 1)
            r = (r * x) % 100;
        x = (x * x) % 100;
        e >>= 1;
    }
    return r;
}
