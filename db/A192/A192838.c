/*
 * A192838  Molecular topological indices of the prism graphs Y_n.
 * https://oeis.org/A192838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*n*(11 + (-1)^n + 2*n*(2 + n))
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 906, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safe guard, though offset is 1 */

    seq_t n_t  = n;
    seq_t base = (n & 1) ? (seq_t)10 : (seq_t)12;          /* 11 + (-1)^n  */
    seq_t inside = base + (seq_t)2 * n_t * ((seq_t)2 + n_t); /* 11+(-1)^n+2*n*(2+n) */
    seq_t num = (seq_t)3 * n_t * inside;                  /* 3*n*(...)      */
    seq_t res = num / (seq_t)2;                           /* divide by 2     */
    return res;
}
