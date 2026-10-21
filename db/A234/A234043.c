/*
 * A234043  a(n) = binomial(5*(n+1),4)/5, with n >= 0.
 * https://oeis.org/A234043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(5*n+2)*(5*n+3)*(5*n+4)/24
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS offset 0 – a(0) = 1 */
seq_t a(idx_t n)
{
    /* Compute each factor as a seq_t to avoid intermediate overflow in idx_t */
    seq_t a1 = (seq_t)(n + 1);
    seq_t b1 = (seq_t)(5 * n + 2);
    seq_t b2 = (seq_t)(5 * n + 3);
    seq_t b3 = (seq_t)(5 * n + 4);

    /* Multiply the four factors and divide by 24.  All operations are
       performed in seq_t, so the code works for built‑in integral types
       (e.g. uint64_t, unsigned __int128) as well as arbitrary‑precision
       integer classes. */
    seq_t result = a1 * b1;
    result *= b2;
    result *= b3;
    result /= 24;

    return result;
}
