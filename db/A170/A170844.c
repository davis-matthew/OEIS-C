/*
 * A170844  G.f.: Product_{k>=0} (1 + 3x^(2^k-1) + x^(2^k)).
 * https://oeis.org/A170844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in
 *   ∏_{k≥0} (1 + 3x^(2^k-1) + x^(2^k))
 *
 * The product is finite for a given n because the exponent 2^k-1 exceeds n
 * for k > ⌊log₂(n+1)⌋.  The algorithm multiplies the factors one by one
 * using dynamic programming: for each factor we keep a temporary array
 * containing the current polynomial; we then add the two terms (with
 * coefficients 3 and 1) shifted by their exponent.  This yields
 * O(n log n) time and O(n) memory.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* number of coefficients needed: from 0 to n inclusive */
    const size_t N = (size_t)n + 1;

    /* allocate two working arrays of type seq_t */
    seq_t *cur = mem_new(seq_t, N);
    seq_t *nxt = mem_new(seq_t, N);
    if (!cur || !nxt) {
        mem_free(cur);
        mem_free(nxt);
        return (seq_t)0;   /* out of memory: return zero */
    }

    /* start with the empty product, i.e. polynomial = 1 */
    cur[0] = (seq_t)1;

    /* iterate over powers of two: 2^k */
    for (idx_t pow2 = 1; pow2 <= n + 1; pow2 <<= 1) {
        idx_t e1 = pow2 - 1;   /* exponent with coefficient 3   */
        idx_t e2 = pow2;       /* exponent with coefficient 1   */

        /* copy current polynomial to nxt (multiplying by the constant term 1) */
        for (size_t i = 0; i < N; ++i)
            nxt[i] = cur[i];

        /* add term 3·x^e1 */
        if (e1 <= n) {
            for (size_t i = (size_t)e1; i < N; ++i)
                nxt[i] += (seq_t)3 * cur[i - e1];
        }

        /* add term 1·x^e2 */
        if (e2 <= n) {
            for (size_t i = (size_t)e2; i < N; ++i)
                nxt[i] += cur[i - e2];
        }

        /* swap cur and nxt for the next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = cur[(size_t)n];

    mem_free(cur);
    mem_free(nxt);
    return result;
}
