/*
 * A147623  The 3rd Witt transform of A040000.
 * https://oeis.org/A147623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(2 - 2*n + n^2) - a(n-1) - a(n-2), with initial values
 *            a(0) = 0, a(1) = 2
 * Range:     u8 n <= 13, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A147623. */
seq_t a(idx_t n)
{
    /* Handle the two explicitly given initial terms. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 2;

    /* Two previous values: a(n-2) and a(n-1). */
    seq_t prev2 = 0;  /* a(0) */
    seq_t prev1 = 2;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Use a signed 64‑bit intermediate to accommodate negative
           terms temporarily.  For n <= 10000 the value of i*i fits
           easily in 64 bits. */
        long long ii = (long long)i;

        /* term = 4 * (2 - 2*i + i^2) */
        long long term_val = 4LL * ((long long)2 - 2LL * ii + ii * ii);
        seq_t term = (seq_t)term_val;

        /* Recurrence:  a(i) = term - prev1 - prev2 */
        cur = term - prev1 - prev2;

        /* Shift for next iteration. */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1; /* cur contains a(n) after the loop finishes. */
}
