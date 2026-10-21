/*
 * A094013  Expansion of (1-4*x)/(1-4*x-4*x^2).
 * https://oeis.org/A094013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Iterative computation of the recurrence */
    seq_t prevprev = (seq_t)1; /* a(0) */
    seq_t prev     = (seq_t)0; /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev * (seq_t)4 + prevprev * (seq_t)4;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
