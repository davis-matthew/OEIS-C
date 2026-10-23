/*
 * A159998  Numerator of Hermite(n, 23/24).
 * https://oeis.org/A159998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 23, a(n) = 23 * a(n‑1) – 288 * (n‑1) * a(n‑2)
 *            for n ≥ 2 (This recurrence follows from
 *            H_{n+1}(x)=2xH_n(x)−2nH_{n‑1}(x) with x=23/24, multiplied by
 *            12^n.)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)23;  /* a(1) */
    seq_t curr;

    for (idx_t k = 2; k <= n; ++k) {
        /* a(k) = 23*a(k-1) – 288*(k-1)*a(k-2)   */
        curr = (seq_t)23 * prev1
             - (seq_t)288 * ( (seq_t)(k - 1) ) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
