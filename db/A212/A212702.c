/*
 * A212702  Main transitions in systems of n particles with spin 7/2.
 * https://oeis.org/A212702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(b-1)*b^(n-1) with b = 8 (Equivalently: a(n) =
 *            n*7*8^(n-1)) Recurrence: a(n) = 16*a(n-1) - 64*a(n-2) for n > 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A212702 */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  Return 0 for out‑of‑range n. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 8^(n-1) by repeated multiplication. */
    seq_t pow8 = (seq_t)1;
    idx_t exp = n - 1U;
    while (exp--)
        pow8 *= (seq_t)8U;

    /* a(n) = n * 7 * 8^(n-1) */
    seq_t result = pow8 * (seq_t)n;
    result *= (seq_t)7U;
    return result;
}
