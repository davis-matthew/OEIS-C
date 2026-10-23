/*
 * A180928  1 + product of any two terms is a triangular number.
 * https://oeis.org/A180928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180928: numbers a(n) such that for all distinct i,j,
   1 + a(i)*a(j) is a triangular number.
   The sequence is finite and known to be: 0, 1, 5, 27.  */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)27;
    return (seq_t)0;   /* terms beyond the finite set are undefined */
}
