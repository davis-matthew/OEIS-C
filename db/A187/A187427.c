/*
 * A187427  Expansion of q^(3/8) * eta(q)^3 / eta(q^3)^4 in powers of q.
 * https://oeis.org/A187427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 33, i32 n <= 111, i64 n <= 387,
 *            i128 n <= 1404, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A187427
 *  a(n) = coefficient of q^n in the product Π_{k>0} (1 - q^k)^3/(1 - q^{3k})^4
 *  The algorithm builds the finite truncation of this product up to exponent n,
 *  using the identity (1 - x^k)^3 = 1 - 3x^k + 3x^{2k} - x^{3k} and
 *  (1 - x^{3k})^{-1} = Σ_{t ≥ 0} p(t) x^{3kt} where p(t) are partition numbers.
 *  Convolution of the two truncated series gives a(n).  O(n^2) operations,
 *  well below 2.0 s for n ≤ 10000, and works for all supported seq_t types.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- 1. Build A(x) = Π_{s≠0 (mod3)} (1 - x^s)^3 ---------- */
    size_t N = (size_t)n;
    seq_t *A = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!A) return 0;               /* memory error */
    A[0] = (seq_t)1;                /* A(0)=1 */

    for (size_t k = 1; k <= N; ++k)
    {
        if (k % 3 == 0) continue;   /* skip multiples of 3   */
        /* Update A in-place:  A(x) *= (1 - x^k)^3 = 1 - 3x^k + 3x^{2k} - x^{3k} */
        for (long long i = (long long)N; i >= 0; --i)
        {
            seq_t val = A[(size_t)i];
            if (i >= (long long)k)      val -= (seq_t)3 * A[(size_t)(i - k)];
            if (i >= (long long)2 * k)  val += (seq_t)3 * A[(size_t)(i - 2 * k)];
            if (i >= (long long)3 * k)  val -= A[(size_t)(i - 3 * k)];
            A[(size_t)i] = val;
        }
    }

    /* ---------- 2. Partition numbers p[0..⌊n/3⌋] using Euler's recurrence ---------- */
    size_t M = N / 3;
    seq_t *P = (seq_t*)calloc(M + 1, sizeof(seq_t));
    if (!P) { free(A); return 0; }
    P[0] = (seq_t)1;
    for (size_t i = 1; i <= M; ++i)
    {
        seq_t sum = (seq_t)0;
        for (int k = 1; ; ++k)
        {
            size_t g1 = (size_t)k * (3 * (size_t)k - 1) / 2;
            size_t g2 = (size_t)k * (3 * (size_t)k + 1) / 2;
            if (g1 > i && g2 > i) break;
            seq_t term = P[i - g1];
            if (k % 2 == 0) term = -term;
            sum += term;
            if (g2 <= i)
            {
                term = P[i - g2];
                if (k % 2 == 0) term = -term;
                sum += term;
            }
        }
        P[i] = sum;
    }

    /* ---------- 3. B(x) = Σ_{t≥0} p(t) x^{3t} ---------- */
    seq_t *B = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!B) { free(A); free(P); return 0; }
    for (size_t t = 0; t <= M; ++t)
        B[3 * t] = P[t];

    /* ---------- 4. Convolution A * B (only j multiple of 3) ---------- */
    seq_t *C = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!C) { free(A); free(P); free(B); return 0; }

    for (size_t i = 0; i <= N; ++i)
    {
        if (A[i] == (seq_t)0) continue;
        seq_t a_val = A[i];
        for (size_t j = 0; j + i <= N; j += 3)   /* B[j] non‑zero only for j%3==0 */
        {
            if (B[j] == (seq_t)0) continue;
            C[i + j] += a_val * B[j];
        }
    }

    seq_t result = C[N];

    /* ---------- 5. Clean up ------------------------------------------- */
    free(A);
    free(P);
    free(B);
    free(C);

    return result;
}
