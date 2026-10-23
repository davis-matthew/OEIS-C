/*
 * A170448  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A170448 – number of reduced words of length n in the
 *  Coxeter group on 7 generators S₁,…,S₇ with relations
 *  (Sᵢ)² = (SᵢSⱼ)⁴⁵ = I.
 *
 *  Generating function (in OEIS notation):
 *    G(t) = (t^45 + 2t^44 + 2t^43 + ⋯ + 2t + 1) /
 *           (1 – 5t – 5t² – ⋯ – 5t⁴⁴ + 15t⁴⁵)
 *
 *  From the denominator we obtain the linear recurrence for n ≥ 45
 *  (order 45):
 *
 *      a(n) = 5·(a(n‑1)+…+a(n‑44)) – 15·a(n‑45)
 *
 *  The first 45 terms (n = 0 … 44) are given by the numerator:
 *
 *      a(0) = 1
 *      a(n) = 5 · (a(0)+…+a(n‑1)) + 2    for 1 ≤ n ≤ 44
 *
 *  The implementation below stores the values in a static cache,
 *  extends it on demand and uses prefix sums to evaluate both
 *  the initial “plus‑2” part and the 45‑term recurrence in O(n)
 *  time for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of already computed terms */
static seq_t *cache   = NULL;
static seq_t *prefix  = NULL;      /* prefix[i] = a(0)+…+a(i) */
static idx_t cache_n  = 0;         /* number of terms stored */

/* extend cache to hold at least up to index 'needed-1' */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_n) return;

    size_t newsize = (size_t)needed;
    cache   = (seq_t *)realloc(cache,   newsize * sizeof(seq_t));
    prefix  = (seq_t *)realloc(prefix,  newsize * sizeof(seq_t));

    for (idx_t i = cache_n; i < needed; ++i) {
        seq_t val;
        if (i == 0) {
            /* a(0) = 1 */
            val = 1;
        } else if (i <= 44) {
            /* a(i) = 5 * prefix[i-1] + 2 , 1 ≤ i ≤ 44 */
            val = 5 * prefix[i-1] + 2;
        } else {
            /* recurrence for n ≥ 45:
             * a(n) = 5 * Σ_{k=n-44}^{n-1} a(k) – 15 * a(n-45)
             */
            seq_t sum_last44 = prefix[i-1] - prefix[i-45];
            val = 5 * sum_last44 - 15 * cache[i-45];
        }

        cache[i] = val;
        if (i == 0) {
            prefix[i] = val;
        } else {
            prefix[i] = prefix[i-1] + val;
        }
    }
    cache_n = needed;
}

/* public function – returns a(n) for any n ≥ 0 */
seq_t a(idx_t n)
{
    grow_cache(n + 1);   /* ensure cache contains a(n) */
    return cache[n];
}
