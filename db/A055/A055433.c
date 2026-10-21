/*
 * A055433  Number of points in Z^n of norm <= 9.
 * https://oeis.org/A055433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{min(n,81)} C(n,k) * 2^k * c_k where c_k = number
 *            of ordered k‑tuples (y_1,…,y_k) with 1 ≤ y_i ≤ 9 and Σ y_i^2 ≤
 *            81. This counts lattice points in ℤ^n whose Euclidean norm is at
 *            most 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 22, u128 n <= 57,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_n_k.h"

/* precomputed constant c[0..81] */
static bool initialized = false;
static seq_t c[82];            /* c_k for k = 0..81 */
static seq_t pow2[82];         /* 2^k for k = 0..81 */

/* initialise c[] and pow2[] once */
static inline void init(void)
{
    if (initialized) return;

    /* dp[k][s] = number of ordered k‑tuples of positive integers (1..9)
       whose squares sum to exactly s (0 ≤ s ≤ 81).              */
    static seq_t dp[82][82];
    for (idx_t k = 0; k <= 81; ++k)
        for (idx_t s = 0; s <= 81; ++s)
            dp[k][s] = 0;
    dp[0][0] = 1;

    for (idx_t k = 0; k < 81; ++k) {
        for (idx_t s = 0; s <= 81; ++s) {
            seq_t cur = dp[k][s];
            if (!cur) continue;
            for (int y = 1; y <= 9; ++y) {
                int t = y * y;                 /* y² */
                idx_t ns = s + t;
                if (ns > 81) break;
                dp[k + 1][ns] += cur;
            }
        }
    }

    for (idx_t k = 0; k <= 81; ++k) {
        seq_t sum = 0;
        for (idx_t s = 0; s <= 81; ++s)
            sum += dp[k][s];
        c[k] = sum;
    }

    pow2[0] = 1;
    for (idx_t k = 1; k <= 81; ++k)
        pow2[k] = pow2[k - 1] * 2;

    initialized = true;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    init();

    seq_t result = 0;
    idx_t maxk = n < 81 ? n : 81;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t binom = binom_n_k(n, k);
        seq_t term = binom * pow2[k] * c[k];
        result += term;
    }
    return result;
}
