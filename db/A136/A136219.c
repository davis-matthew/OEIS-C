/*
 * A136219  Number of terms in rows of irregular triangle A136218.
 * https://oeis.org/A136219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^2 + 3*n + 2)/2 * - Σ_{k: b_k ≤ n} (n - b_k + 1), *
 *            where b_0 = 1 and b_k = 2*b_{k-1} - ⌊(k+1)/3⌋ for k ≥ 1
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* 3*n^2 + 3*n + 2 is always even. */
    seq_t up = (3 * N * N + 3 * N + 2) / 2;

    seq_t sum = 0;
    seq_t b = 1;
    if (b <= N)
        sum += (N - b + 1);

    for (idx_t k = 1; ; ++k) {
        seq_t sub = (seq_t)((k + 1) / 3);
        b = 2 * b - sub;
        if (b > N)
            break;
        sum += (N - b + 1);
    }

    return up - sum;
}
