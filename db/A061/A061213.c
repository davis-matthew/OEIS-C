/*
 * A061213  a(n) = 1 + product of first n positive triangular numbers
 *          (A000217).
 * https://oeis.org/A061213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + product_{k=1}^n (k*(k+1)/2) Recurrence: a(1) = 2 a(n)
 *            = (n*(n+1)/2) * (a(n-1)-1) + 1 for n >= 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe default. */
    if (n < 1)
        return 0;

    seq_t a_prev = 2;                /* a(1) */

    if (n == 1)
        return a_prev;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute the i‑th triangular number in seq_t form. */
        seq_t Ti = ((seq_t)i * ((seq_t)i + 1)) / 2;
        /* Apply the recurrence. */
        seq_t a_curr = Ti * (a_prev - 1) + 1;
        a_prev = a_curr;
    }

    return a_prev;
}
