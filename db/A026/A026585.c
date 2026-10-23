/*
 * A026585  a(n) = T(n,n), T given by A026584. Also a(n) is the number of
 *          integer strings s(0), ..., s(n) counted by T, such that s(n)=0.
 * https://oeis.org/A026585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n‑k‑1, n‑2k) * C(2k, k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (C(·,·) is the binomial coefficient.  For n=0 the value is 1.) */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = 0;
    idx_t half = n / 2;
    if (half == 0) {          /* n == 1 */
        return result;        /* result is 0 */
    }

    /* Initialise for k = 1 */
    seq_t B = (seq_t)1;            /* C(n‑1‑1, n‑2) = C(n‑2, n‑2) = 1 */
    seq_t L = (seq_t)2;            /* C(2, 1) = 2 */
    result += B * L;

    /* Iterate for k = 2 .. half */
    for (idx_t k = 2; k <= half; ++k) {
        /* Update B: B_k+1 = B_k * (n‑2k)*(n‑2k‑1) / (k*(n‑k‑1)) */
        seq_t num = (seq_t)(n - 2 * (k - 1)) * (seq_t)(n - 2 * (k - 1) - 1);
        seq_t den = (seq_t)(k - 1) * (seq_t)(n - (k - 1) - 1);
        B = B * num / den;

        /* Update L: L_k+1 = L_k * (4k‑2) / (k+1)  (k is previous step) */
        L = L * (seq_t)(4 * (k - 1) + 2) / (seq_t)k;

        result += B * L;
    }

    return result;
}
