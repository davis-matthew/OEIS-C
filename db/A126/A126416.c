/*
 * A126416  Number of base 29 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126416
 * a(n) counts length‑n strings over the alphabet {0,…,28}
 * whose adjacent symbols differ by ≤2.
 * DP recurrence with 29 states:
 *   dp[1][d] = 1                 (0 ≤ d ≤ 28)
 *   dp[pos][d] = Σ dp[pos‑1][p]  for p ∈ [max(0,d‑2), min(28,d+2)]
 *   a(n) = Σ dp[n][d]            (0 ≤ d ≤ 28)
 * Initial values: a(0)=1, a(1)=29.
 */
seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)29;

    const idx_t D = 29u;                     /* number of states (digits 0‑28) */
    seq_t prev[D];
    seq_t curr[D];

    /* Base case: pos = 1 */
    for (idx_t i = 0; i < D; ++i) prev[i] = (seq_t)1;

    /* Iterate positions 2 … n */
    for (idx_t pos = 2; pos <= n; ++pos) {
        /* Zero out current state's array */
        for (idx_t i = 0; i < D; ++i) curr[i] = (seq_t)0;

        /* Compute transitions for each digit */
        for (idx_t d = 0; d < D; ++d) {
            idx_t start = (d >= 2u) ? (d - 2u) : 0u;
            idx_t end   = ((d + 2u) < (D - 1u)) ? (d + 2u) : (D - 1u);
            seq_t sum = 0;
            for (idx_t p = start; p <= end; ++p) sum += prev[p];
            curr[d] = sum;
        }

        /* Copy current to previous for next iteration */
        for (idx_t i = 0; i < D; ++i) prev[i] = curr[i];
    }

    /* Sum up all ending states to get a(n) */
    seq_t total = 0;
    for (idx_t d = 0; d < D; ++d) total += prev[d];
    return total;
}
