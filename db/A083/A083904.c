/*
 * A083904  Expansion of g.f. 1/(1-x) * Sum_{k>=0} 3^k * x^2^(k+1)/(1+x^2^k).
 * https://oeis.org/A083904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; For n ≥ 2: a(2n) = 3*a(n) + 1 a(2n+1) = 3*a(n) These
 *            rules are implemented recursively
 * Range:     u8 n <= 63, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case – the first term in the sequence */
    if (n <= 1)
        return 0;

    /* Recursive step: use the value for floor(n/2) */
    seq_t half = a(n >> 1);

    /* Even or odd determination from the least‑significant bit */
    if ((n & 1ULL) == 0ULL)
        return (seq_t)3 * half + 1;
    else
        return (seq_t)3 * half;
}
