/*
 * A100744  Inverse modulo 2 binomial transform of (-2)^n.
 * https://oeis.org/A100744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-2)^n - Σ_{k ⊂ n, k<n} a(k) The sum runs over all proper
 *            subsets of the binary representation of n. Computation uses the
 *            inverse modulo‑2 binomial transform
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t a_val[10001];
static idx_t computed = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (!initialized) {
        a_val[0] = 1;
        computed = 0;
        initialized = true;
    }

    /* Compute anew up to requested n */
    if (n > computed) {
        for (idx_t m = computed + 1; m <= n; ++m) {
            seq_t sum_sub = 0;
            /* Enumerate all proper subsets of m */
            uint64_t bits = m;
            uint64_t k = bits;
            while (1) {
                if (k != bits) {           /* proper subset */
                    sum_sub += a_val[k];
                }
                if (k == 0) break;
                k = (k - 1) & bits;
            }
            seq_t pow2n = pow2(m);         /* 2^m */
            seq_t term = ((m & 1) ? -pow2n : pow2n); /* (-2)^m */
            a_val[m] = term - sum_sub;
        }
        computed = n;
    }

    return a_val[n];
}
