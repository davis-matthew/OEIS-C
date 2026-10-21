/*
 * A213593  Stirling transform of the first kind of the Lucas numbers A000032.
 * https://oeis.org/A213593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=2, a(1)=1, a(n+2) = (1+n-n^2)*a(n) - 2*n*a(n+1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)1;

    for (idx_t k = 0; k + 2 <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t coeff = (seq_t)1 + k_seq - k_seq * k_seq;
        seq_t a_next = coeff * a0 - (seq_t)2 * k_seq * a1;
        a0 = a1;
        a1 = a_next;
    }

    return a1;
}
