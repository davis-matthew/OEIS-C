/*
 * A070609  a(n) = n^5 mod 25.
 * https://oeis.org/A070609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 25;
    /* Reduce n modulo 25 first – the exponent is small, so overflow is impossible. */
    seq_t r = (seq_t)(n % 25);
    seq_t res = 1;
    /* Compute r to the 5th power modulo 25. */
    for (int i = 0; i < 5; ++i) {
        res = (res * r) % MOD;
    }
    return res;
}
