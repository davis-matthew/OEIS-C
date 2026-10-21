/*
 * A094433  a(n) is the left term in M^n * [1 0 0], M = the 3 X 3 matrix [1 -1
 *          0 / -1 3 -2 / 0 -2 2].
 * https://oeis.org/A094433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * a(n-1) - 6 * a(n-2) with initial values a(0) = 1,
 *            a(1) = 1, a(2) = 2. (Equivalent to a(n) = (3+√3)^(n-2) +
 *            (3-√3)^(n-2) for n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    /* Use the recurrence a(k) = 6*a(k-1) - 6*a(k-2) for k ≥ 3. */
    seq_t a_prevprev = (seq_t)1; /* a(0) */
    seq_t a_prev     = (seq_t)1; /* a(1) */
    seq_t a_curr     = (seq_t)2; /* a(2) */

    const seq_t six = (seq_t)6;

    /* Iterate from k = 3 up to n. */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t next = (a_curr - a_prev) * six;  /* 6*(a(k-1) - a(k-2)) = 6*a(k-1) - 6*a(k-2) */
        a_prevprev = a_prev;
        a_prev     = a_curr;
        a_curr     = next;
    }

    return a_curr;
}
