/*
 * A034715  Dirichlet convolution of triangular numbers with themselves.
 * https://oeis.org/A034715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} T(d) * T(n/d), where T(k) = k * (k + 1) / 2 is
 *            the k‑th triangular number
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            seq_t Ti = (seq_t)i * ((seq_t)i + 1) / 2;
            seq_t Tj = (seq_t)j * ((seq_t)j + 1) / 2;
            seq_t prod = Ti * Tj;
            result += prod;               /* d = i */
            if (i != j) {
                result += prod;           /* d = j (distinct) */
            }
        }
    }
    return result;
}
