/*
 * A017105  a(n) = (8*n+3)^5.
 * https://oeis.org/A017105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10, u64 n <= 891,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index n to seq_t before the calculation. */
    seq_t t = (seq_t)8 * n + (seq_t)3;   /* t = 8*n + 3 */
    seq_t t2 = t * t;                    /* t^2 */
    seq_t t4 = t2 * t2;                  /* t^4 */
    seq_t result = t4 * t;               /* t^5 = (8*n+3)^5 */
    return result;
}
