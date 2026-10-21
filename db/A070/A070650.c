/*
 * A070650  a(n) = n^6 mod 27.
 * https://oeis.org/A070650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 27
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 27 – all further work stays in
       the range 0 .. 26 so that intermediate products are tiny. */
    seq_t x = (seq_t)(n % 27);   /* x now fits in a byte (≤26) */

    /* Compute x^6 modulo 27 by repeated multiplication. */
    seq_t r = 1;
    for (int i = 0; i < 6; ++i) {
        r = (r * x) % 27;
    }
    return r;
}
