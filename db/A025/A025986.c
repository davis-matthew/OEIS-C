/*
 * A025986  Expansion of 1/((1-2*x)*(1-5*x)*(1-6*x)*(1-7*x)).
 * https://oeis.org/A025986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(n+1)-2^(n+1))/3 + 13*a(n-1) - 42*a(n-2); Initial
 *            values: a(0) = 1, a(1) = 20
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 20;

    /* Current powers: 2^k and 5^k, starting with k = 2 (needed for a(2)) */
    seq_t pow2 = 4;   /* 2^2 */
    seq_t pow5 = 25;  /* 5^2 */

    seq_t prev0 = 1;  /* a(0) */
    seq_t prev1 = 20; /* a(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* Next powers for the recurrence: 2^(k+1) and 5^(k+1) */
        seq_t pow2_next = pow2 * 2;
        seq_t pow5_next = pow5 * 5;

        /* (5^(k+1) - 2^(k+1))/3  part of the recurrence */
        seq_t part = (pow5_next - pow2_next) / 3;

        /* a(k) = part + 13*a(k-1) - 42*a(k-2) */
        seq_t curr = part + 13 * prev1 - 42 * prev0;

        /* Advance for next iteration */
        prev0 = prev1;
        prev1 = curr;
        pow2 = pow2_next;
        pow5 = pow5_next;
    }

    return prev1;
}
