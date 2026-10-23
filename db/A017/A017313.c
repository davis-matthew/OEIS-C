/*
 * A017313  a(n) = (10*n + 3)^9.
 * https://oeis.org/A017313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 13, u128 n <= 1910,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A017313: a(n) = (10*n + 3)^9.
   The function is independent of call order and works for any unsigned
   index n. */
seq_t a(idx_t n)
{
    /* Compute the base value 10*n + 3 in the result type.
       All arithmetic uses the seq_t type to avoid overflow in intermediate
       conversions. */
    seq_t base = (seq_t)n * 10 + (seq_t)3;

    /* Compute the 9th power by repeated multiplication.
       9 iterations are trivial and run in O(1) time. */
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i)
        result *= base;

    return result;
}
