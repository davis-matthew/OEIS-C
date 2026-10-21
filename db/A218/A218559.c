/*
 * A218559  Sum_{i=0..n-1} i*(n^(i+1)-1)/(n-1)*n^(i(i+1)/2).
 * https://oeis.org/A218559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{n-1} i * (n^{i+1}-1)/(n-1) * n^{i(i+1)/2}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    seq_t base = (seq_t)n;   /* n as seq_t */

    seq_t total = (seq_t)0;  /* running sum of terms */
    seq_t S     = (seq_t)1;  /* S_i = Σ_{k=0}^{i} n^k, starts at i=0 */
    seq_t npow  = (seq_t)1;  /* n^i, starts at i=0 */
    seq_t powB  = (seq_t)1;  /* n^{i(i+1)/2}, starts at i=0 */

    for (idx_t i = 0; i < n; ++i) {
        /* term for current i */
        seq_t term = (seq_t)i * S;  /* i * S_i */
        term *= powB;              /* i * S_i * n^{i(i+1)/2} */
        total += term;             /* accumulate */

        /* prepare values for next i (i → i+1) */
        seq_t next_powB = powB * npow * base; /* n^{(i+1)(i+2)/2} = n^{i(i+1)/2} * n^{i+1} */
        seq_t next_npow = npow * base;        /* n^{i+1} */
        seq_t next_S    = S * base + (seq_t)1;/* Σ_{k=0}^{i+1} n^k */

        powB = next_powB;
        npow = next_npow;
        S    = next_S;
    }

    return total;
}
