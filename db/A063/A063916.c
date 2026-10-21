/*
 * A063916  G.f.: (1 + Sum_{ i >= 0 } 2^i*x^(2^(i+1)-1)) / (1-x)^3.
 * https://oeis.org/A063916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n+2,2) + Σ_{i≥0, 2^{i+1}-1 ≤ n}
 *            binom(n-(2^{i+1}-1)+2,2) * 2^i
 * Range:     u8 n <= 10, u16 n <= 79, u32 n <= 3276, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where binom(m,2) = m*(m-1)/2 and n is a non‑negative integer index. */

seq_t a(idx_t n)
{
    /* base term binom(n+2,2) = (n+2)(n+1)/2 */
    seq_t result = ((seq_t)n + 2) * ((seq_t)n + 1) / 2;

    /* iterate over i such that 2^{i+1}-1 ≤ n */
    for (idx_t i = 0; ; ++i)
    {
        /* 2^{i+1} */
        idx_t pow = (idx_t)1 << (i + 1);
        /* if pow would overflow or be > n+1, stop */
        if (pow == 0 || pow > n + 1)
            break;

        idx_t sub = pow - 1;          /* 2^{i+1}-1 */
        if (sub > n)
            break;

        idx_t offset = n - sub;       /* n - (2^{i+1}-1) */
        seq_t tri = ((seq_t)offset + 2) * ((seq_t)offset + 1) / 2;
        seq_t term = tri * ((seq_t)1 << i);   /* times 2^i */
        result += term;
    }

    return result;
}
