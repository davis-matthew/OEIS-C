/*
 * A208817  Number of n-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2n)) * ( Σ_{d|n} φ(n/d) * F(d) + Ref(n) ), * * F(k)
 *            is the number of k‑tuples of integers from {–n,…,n} whose sum is
 *            0 * and is computed by the inclusion–exclusion formula * * F(k)
 *            = Σ_{j=0}^{⌊k·n/(2n+1)⌋} (–1)^j · C(k,j) · C(k·n – j·(2n+1) + k
 *            – 1, k – 1). * * Ref(n) is the total cont...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   binom_small_uint, ways_sum_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom_small_uint.h"
#include "../utils/ways_sum_k.h"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Euler’s totient function φ(m) – fast enough for m ≤ 10⁵                    */
static seq_t phi_idx(idx_t m)
{
    seq_t res = (seq_t)m;
    idx_t x = m;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            res -= res / p;
        }
    }
    if (x > 1) res -= res / x;
    return res;
}

/* -------------------------------------------------------------------------- */
/* pairCount(t): number of ordered pairs (a,b) in [–n,n] with a+b=t            */
static inline seq_t pair_count(idx_t n, int t)
{
    int low  = (t - (int)n > -(int)n) ? t - (int)n : -(int)n;
    int high = (t + (int)n <  (int)n) ? t + (int)n :  (int)n;
    return (high >= low) ? (seq_t)(high - low + 1) : (seq_t)0;
}

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Reflection contribution for even n (type A)                                 */
static seq_t refl_typeA(idx_t n)
{
    /* n is even: n = 2*m, m ≥ 1 */
    idx_t m = n / 2;
    idx_t k = m - 1;               /* number of pairs of symmetric positions */

    seq_t sum = 0;
    for (int S = -(int)n; S <= (int)n; ++S) {
        seq_t pc = pair_count(n, -2 * S);
        if (pc == 0) continue;
        seq_t fk = ways_sum_k(k, S, n);
        if (fk == 0) continue;
        sum += pc * fk;
    }
    return sum;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* safety for out‑of‑offset values */

    /* ---------- Rotations -------------------------------------------------- */
    seq_t rot_sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) if (n % d == 0) {
        idx_t d1 = d;
        idx_t d2 = n / d;

        seq_t f_d1 = ways_sum_k(d1, 0, n);
        seq_t phi1  = phi_idx(n / d1);
        rot_sum += phi1 * f_d1;

        if (d1 != d2) {
            seq_t f_d2 = ways_sum_k(d2, 0, n);
            seq_t phi2  = phi_idx(n / d2);
            rot_sum += phi2 * f_d2;
        }
    }

    /* ---------- Reflections ------------------------------------------------ */
    seq_t refl_sum = 0;
    if (n & 1) {                     /* n odd */
        idx_t m = (n - 1) / 2;
        seq_t sum = 0;
        for (int t = -(int)m; t <= (int)m; ++t) {
            sum += ways_sum_k(m, t, n);
        }
        refl_sum = (seq_t)n * sum;
    } else {                         /* n even */
        idx_t m = n / 2;
        seq_t typeA = refl_typeA(n);
        seq_t typeB = ways_sum_k(m, 0, n);
        refl_sum = (seq_t)m * (typeA + typeB);
    }

    /* ---------- Final Burnside count -------------------------------------- */
    seq_t total = rot_sum + refl_sum;
    seq_t denom = (seq_t)(2 * n);
    return total / denom;
}
