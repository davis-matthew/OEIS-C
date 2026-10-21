/*
 * A225399  Number of nontrivial triangular numbers dividing triangular(n).
 * https://oeis.org/A225399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 2 ≤ m < n : m(m+1) | n(n+1) }
 * Range:     u8 n <= 16, u16 n <= 1166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of triangular numbers T(m) with 1 < T(m) < T(n) such that T(m) | T(n). */

seq_t a(idx_t n)
{
    /* For n < 3 no nontrivial triangular divisors exist. */
    if (n < 3) return 0;

    /* N = n(n+1). We check m(m+1) | N for 2 ≤ m < n. */
    seq_t N = (seq_t)n * ((seq_t)n + 1);
    seq_t count = 0;

    for (idx_t m = 2; m < n; ++m) {
        seq_t mseq = (seq_t)m;
        if (N % mseq == 0 && N % (mseq + 1) == 0)
            ++count;
    }

    return count;
}
