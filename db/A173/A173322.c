/*
 * A173322  a(n) = 4*n! + 1.
 * https://oeis.org/A173322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n! + 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes n! iteratively to avoid any implicit factorial function. */
seq_t a(idx_t n)
{
    seq_t fact = (seq_t)1;          /* fact holds the running factorial */
    for (idx_t i = 2; i <= n; ++i)  /* multiply by 2..n for n ≥ 2 */
        fact *= (seq_t)i;           /* cast to seq_t to preserve type */

/* a(n) = 4 * n! + 1   (cast to seq_t to keep operations in seq_t)  */
    return fact * (seq_t)4 + (seq_t)1;
}
