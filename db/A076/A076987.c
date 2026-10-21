/*
 * A076987  Smallest triangular number of the form n*k + 1 with k>0.
 * https://oeis.org/A076987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest triangular number T_m = m*(m+1)/2 such that
 *            T_m = n*k + 1 for some integer k > 0 (i.e., T_m ≡ 1 (mod n)).
 *            This implementation performs a linear search over m starting
 *            from 2 until the congruence is satisfied. It works for any
 *            unsigned integer type seq_t (including arbitrary‑pr...
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the sequence definition. */
    if (n == 0)
        return (seq_t)0;          /* defensive: unused case */

    /* Start from m = 2 because T1 = 1 gives k = 0, which is not allowed. */
    for (idx_t m = 2; ; ++m) {
        seq_t t = (seq_t)m * ((seq_t)m + (seq_t)1) / (seq_t)2;
        /* Check if t ≡ 1 (mod n) and k = (t-1)/n > 0. */
        if (t > (seq_t)1 && ((t - (seq_t)1) % (seq_t)n) == (seq_t)0)
            return t;
    }
}
