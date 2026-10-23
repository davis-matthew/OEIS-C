/*
 * A194826  Units' digits of the nonzero 9-gonal (nonagonal) numbers.
 * https://oeis.org/A194826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n*(7*n - 5)/2) mod 10 (units digit of the nth nonzero
 *            9‑gonal number)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Work modulo 20 to avoid overflow, then halve and take modulo 10. */
    idx_t n20 = n % 20u;                     /* n mod 20 (0..19) */
    idx_t m20 = (7 * n20 + 15) % 20u;         /* (7*n - 5) mod 20 */
    idx_t r  = (n20 * m20) % 20u;             /* n*(7*n-5) mod 20 */
    seq_t result = (seq_t)(r / 2);            /* r is even; divide by 2 gives the desired digit */
    return result;
}
