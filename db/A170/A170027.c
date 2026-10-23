/*
 * A170027  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 ≤ n ≤ 35: a(n) = 2 + 16 * Σ_{k=0}^{n-1} a(k) for
 *            n = 36: a(36) = 1 + 16 * Σ_{k=0}^{35} a(k) - 136 * a(0) for n ≥
 *            37: a(n) = 16 * Σ_{k=1}^{35} a(n-k) - 136 * a(n-36) (these
 *            formulas follow from the rational generating function in the
 *            OEIS entry)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 10000  /* sufficient for the required range */

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t a_vals[MAX_N + 1];
    static seq_t cum[MAX_N + 1];
    static idx_t computed = 0;

    if (!ready) {
        a_vals[0] = (seq_t)1;
        cum[0]   = (seq_t)1;
        computed = 0;
        ready    = true;
    }

    if (n > MAX_N) {
        /* The requested index is larger than the pre‑allocated range.
           For the intended usage (n ≤ 10000) this should not happen.
           If it does, extend the range safely. */
        if (n > (idx_t)MAX_N) {
            /* If larger than the allowed maximum, abort or clamp. */
            return (seq_t)0;
        }
    }

    if (n <= computed)
        return a_vals[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sumPrev;
        seq_t value;

        if (i <= 35) {                 /* 1 ≤ i ≤ 35 */
            sumPrev = cum[i - 1];
            value   = (seq_t)2 + (seq_t)16 * sumPrev;
        } else if (i == 36) {           /* i = 36 */
            sumPrev = cum[35];
            value   = (seq_t)1 + (seq_t)16 * sumPrev - (seq_t)136 * a_vals[0];
        } else {                        /* i ≥ 37 */
            sumPrev = cum[i - 1] - cum[i - 36];
            value   = (seq_t)16 * sumPrev - (seq_t)136 * a_vals[i - 36];
        }

        a_vals[i] = value;
        cum[i]    = cum[i - 1] + value;
    }

    computed = n;
    return a_vals[n];
}
