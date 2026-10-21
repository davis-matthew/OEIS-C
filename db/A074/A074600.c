/*
 * A074600  a(n) = 2^n + 5^n.
 * https://oeis.org/A074600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp for any integer type seq_t and index type idx_t.
   Works for both 64‑bit unsigned integers and arbitrary‑precision
   integer classes. */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Returns the n‑th term of sequence A074600 with offset 0. */
seq_t a(idx_t n) {
    seq_t p2 = pow_seq_t((seq_t)2, n);
    seq_t p5 = pow_seq_t((seq_t)5, n);
    return p2 + p5;
}
