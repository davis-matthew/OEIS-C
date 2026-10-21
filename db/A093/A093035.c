/*
 * A093035  Number of triples (d1,d2,d3) where each element is a divisor of n
 *          and d1 + d2 + d3 <= n.
 * https://oeis.org/A093035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{(d1,d2,d3) | each di | n and d1 + d2 + d3 ≤ n} (ordered
 *            triples of divisors of n whose sum does not exceed n)
 * Range:     u8 n <= 23, u16 n <= 2519, u32 n <= 8175, u64 n <= 8175,
 *            u128 n <= 8175, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /*--- 1. collect all divisors of n ----------------------------------*/
    int divs[256];          /* 256 is safely larger than any divisor count
                               for n ≤ 10000   (max 200)                */
    int div_cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (div_cnt < 256) divs[div_cnt++] = (int)d;
            if (d * d != n && div_cnt < 256)
                divs[div_cnt++] = (int)(n / d);
        }
    }

    /*--- 2. build pair-sum frequencies --------------------------------*/
    idx_t pair_len = 2 * n + 1;                 /* sums up to 2*n */
    uint64_t *paircnt = (uint64_t *)malloc(pair_len * sizeof(uint64_t));
    if (!paircnt) return (seq_t)0;              /* unlikely, but safety */
    memset(paircnt, 0, pair_len * sizeof(uint64_t));

    for (int i = 0; i < div_cnt; ++i) {
        uint64_t di = (uint64_t)divs[i];
        for (int j = 0; j < div_cnt; ++j) {
            uint64_t sum = di + (uint64_t)divs[j];
            if (sum <= (uint64_t)(2 * n))
                ++paircnt[sum];
        }
    }

    /*--- 3. prefix sums of paircnt -----------------------------------*/
    uint64_t *prefix = (uint64_t *)malloc(pair_len * sizeof(uint64_t));
    if (!prefix) { free(paircnt); return (seq_t)0; }
    uint64_t acc = 0;
    for (idx_t s = 0; s < pair_len; ++s) {
        acc += paircnt[s];
        prefix[s] = acc;
    }

    /*--- 4. count triples ---------------------------------------------*/
    uint64_t ans = 0;
    for (int k = 0; k < div_cnt; ++k) {
        uint64_t d3 = (uint64_t)divs[k];
        uint64_t limit = (uint64_t)n - d3;                /* maximum allowed d1+d2 */
        if (limit > (uint64_t)2 * n) limit = (uint64_t)2 * n;
        ans += prefix[limit];
    }

    /*--- 5. cleanup and return ----------------------------------------*/
    free(paircnt);
    free(prefix);

    return (seq_t)ans;
}
