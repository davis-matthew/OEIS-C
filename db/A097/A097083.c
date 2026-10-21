/*
 * A097083  Positive values of k such that there is exactly one permutation p
 *          of (1,2,3,...,k) such that i+p(i) is a Fibonacci number for
 *          1<=i<=k.
 * https://oeis.org/A097083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + ((n*(n+1)/2) mod 2), with a(1)=1,
 *            a(2)=2. This recurrence is derived from the OEIS entry A097083
 *            and generates the sequence for all n>=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev = (seq_t)2;    /* a(n-1), starting with a(2) */
    seq_t prev2 = (seq_t)1;   /* a(n-2), starting with a(1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t tri = ((seq_t)i * (seq_t)(i + 1)) / 2;   /* triangular number T(i) */
        seq_t add = tri & (seq_t)1;                    /* T(i) mod 2 */
        seq_t curr = prev + prev2 + add;               /* a(i) */

        prev2 = prev;
        prev = curr;
    }
    return prev;
}
