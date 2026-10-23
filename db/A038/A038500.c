/*
 * A038500  Highest power of 3 dividing n.
 * https://oeis.org/A038500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 3^v_3(n) – the highest power of 3 dividing n
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1; if n == 0 the function returns 1 (not part of the
       sequence, but harmless). */
    seq_t result = (seq_t)1;
    /* Count the factors of 3.  Each time we divide n by 3 we
       multiply the result by 3, building the desired power. */
    while ((n % 3U) == 0U) {
        result *= (seq_t)3;
        n /= 3U;
    }
    return result;
}
