/*
 * A097900  Number of runs of length 1 in all permutations of [n]. (The
 *          permutation 3574162 has two runs of length 1: 357/4/16/2.)
 * https://oeis.org/A097900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n+4)/6 for n>=2, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and the special small case */
    if (n <= 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* For n>=3, n! is divisible by 6, so we can divide early
       to keep the intermediate product within 64‑bit bounds when
       seq_t is 64 bits. */
    fact /= 6;

    /* Finish computation: fact * (n+4) */
    fact *= (seq_t)(n + 4);

    return fact;
}
