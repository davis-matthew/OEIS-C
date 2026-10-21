/*
 * A055947  n - reversal of base 3 digits of n (written in base 10).
 * https://oeis.org/A055947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 134, i16 n <= 9429, i32 n <= 9429, i64 n <= 9429,
 *            i128 n <= 9429, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A055947: a(n) = n - A030102(n) 
 * where A030102(n) is the reversal of the base‑3 representation of n.
 * Computes the value directly by reversing the base‑3 digits of n.
 */
seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t t = n;
    while (t > 0) {
        rev = rev * 3 + (seq_t)(t % 3);
        t /= 3;
    }
    return (seq_t)n - rev;
}
