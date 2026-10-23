/*
 * A025945  Expansion of 1/((1-2*x)*(1-3*x)*(1-7*x)*(1-10*x)).
 * https://oeis.org/A025945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A025945
   Expansion of 1/((1-2*x)*(1-3*x)*(1-7*x)*(1-10*x))
   Recurrence: a(n) = 3^(n+1) - 2^(n+1) + 17*a(n-1) - 70*a(n-2)
   Initial values: a(0)=1, a(1)=22
   The function returns the nth term for any n ≥ 0.
   All arithmetic is performed in seq_t so the result is exact
   whenever it fits in the target type. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)22;

    /* previous two terms */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)22;  /* a(1) */

    /* powers needed for the recurrence:
       p2p = 2^(i+1), p3p = 3^(i+1)  at the start of each iteration i  */
    seq_t p2p = (seq_t)8;  /* 2^3 for i = 2 */
    seq_t p3p = (seq_t)27; /* 3^3 for i = 2 */

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* compute the term 3^(i+1) - 2^(i+1) */
        seq_t term = p3p - p2p;

        /* recurrence: a(i) = term + 17*a(i-1) - 70*a(i-2) */
        seq_t a_cur = term + (seq_t)17 * a_prev1 - (seq_t)70 * a_prev2;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;

        /* update powers for the next i */
        if (i == n) break;          /* finished, no need to update */
        p2p *= (seq_t)2;            /* 2^(i+2) = 2^(i+1) * 2 */
        p3p *= (seq_t)3;            /* 3^(i+2) = 3^(i+1) * 3 */
    }
    return a_prev1;
}
