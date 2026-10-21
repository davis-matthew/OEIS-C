/*
 * A053985  Replace 2^k with (-2)^k in binary expansion of n.
 * https://oeis.org/A053985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = sum_{i} bit_i(n) * (-2)^i
 * Range:     i8 n <= 129, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bit_i(n) is the i‑th bit of n (i=0 LSB). */
seq_t a(idx_t n) {
    seq_t result = 0;   /* equivalent to a(0) = 0        */
    seq_t term   = 1;   /* (-2)^0                         */
    while (n) {
        if (n & 1) result += term;   /* add current power of (-2) if bit=1 */
        term = term * -2;            /* advance to the next power: (-2)^(k+1) */
        n >>= 1;                     /* move to the next bit          */
    }
    return result;
}
