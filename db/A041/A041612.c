/*
 * A041612  Numerators of continued fraction convergents to sqrt(325).
 * https://oeis.org/A041612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=18, a(1)=649, a(n)=36*a(n-1)+a(n-2) for n>1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two defined initial values directly */
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)649;

    /* Recurrence: a(n) = 36*a(n-1) + a(n-2) */
    seq_t prev2 = (seq_t)18;      /* a(0) */
    seq_t prev1 = (seq_t)649;     /* a(1) */
    seq_t curr;

    /* Iteratively compute up to a(n) using only O(1) memory */
    for (idx_t i = 2; i <= n; ++i) {
        curr = (prev1 * (seq_t)36) + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;  /* when i == n, prev1 holds a(n) */
}
