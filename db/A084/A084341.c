/*
 * A084341  13*n digit-reversed mod 13.
 * https://oeis.org/A084341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(13*n) mod 13
 * Range:     u8 n <= 7, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute 13 * n */
    seq_t prod = (seq_t)n * (seq_t)13;

    /* Reverse the decimal representation of prod.
       This works for any integral type seq_t (fixed size or big integer). */
    seq_t rev = 0;
    seq_t tmp = prod;
    while (tmp > 0) {
        rev = rev * (seq_t)10 + (tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Return the remainder modulo 13. */
    return rev % (seq_t)13;
}
