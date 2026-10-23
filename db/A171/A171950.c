/*
 * A171950  a(1)=1. a(n) = the absolute difference between (the sum of
 *          previous terms) and A000217(n-2), n>1.
 * https://oeis.org/A171950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = |∑_{i=1}^{n-1} a(i) - T(n-2)| (T(k) = k·(k+1)/2)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Closed form: a(n) = n-2 for all n ≥ 5. */

/* The function implements these rules and works for any unsigned
   seq_t type (including arbitrary‑precision integer classes).  It
   requires no dynamic memory or helper functions. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0
       (the value is irrelevant because n = 0 is outside the
       official offset). */
    if (n == 0)
        return (seq_t)0;

    /* Handle the first four terms explicitly (they do not follow
       the closed form n-2). */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)0;

    /* For n ≥ 5 the closed form a(n) = n-2 holds exactly. */
    return (seq_t)n - (seq_t)2;
}
