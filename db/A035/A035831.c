/*
 * A035831  Coordination sequence for lattice D*_92 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{92} 2^k · binom(92, k) · binom(n‑1, k‑1) + 2^92
 *            · binom(n+45, 91)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b)  */
#include "../utils/pow2.h"    /* provides seq_t pow2(idx_t exp)      */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* static tables for the fixed parameters (n = 92)          */
    static bool init = false;
    static seq_t comb92[93];
    static seq_t pow2k[93];
    static seq_t pow2_92;

    if (!init) {
        for (idx_t k = 0; k <= 92; ++k) {
            comb92[k] = binom(92, k);
            pow2k[k]   = pow2(k);
        }
        pow2_92    = pow2(92);
        init       = true;
    }

    seq_t sum = 0;
    /* k = 0 contributes only for n == 0, handled above            */
    for (idx_t k = 1; k <= 92; ++k) {
        if (k > n) break;          /* binom(n‑1, k‑1) would be 0          */
        seq_t c1 = comb92[k];
        seq_t c2 = binom(n - 1, k - 1);
        /* c2 may be 0; multiplication will then contribute 0   */
        sum += c1 * c2 * pow2k[k];
    }

    seq_t second = pow2_92 * binom(n + 45, 91);
    return sum + second;
}
