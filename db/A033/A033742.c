/*
 * A033742  Expansion of Product_{d | 30} theta_3(q^d).
 * https://oeis.org/A033742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|30} θ₃(q^d) where θ₃(q) = 1 +
 *            2∑_{k≥1} q^{k²} The divisors of 30 are 1,2,3,5,6,10,15,30
 * Range:     u8 n <= 23, u16 n <= 143, u32 n <= 5519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required external types: seq_t and idx_t are supplied by the build. */
#include <stdlib.h>   /* malloc, calloc, free */
#include <stdint.h>   /* for uint64_t, if needed */

#define NUM_DIV 8
static const idx_t DIVISORS[NUM_DIV] = {1, 2, 3, 5, 6, 10, 15, 30};

seq_t a(idx_t n)
{
    /* handle the trivial case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* allocate two coefficient arrays of length n+1 */
    size_t len = (size_t)n + 1;
    seq_t *res = (seq_t *)calloc(len, sizeof(seq_t));
    if (!res) return (seq_t)0;
    seq_t *new_res = (seq_t *)calloc(len, sizeof(seq_t));
    if (!new_res) {
        free(res);
        return (seq_t)0;
    }

    /* start with the constant polynomial 1 */
    res[0] = (seq_t)1;

    /* iterate over all divisors */
    for (int di = 0; di < NUM_DIV; ++di) {
        idx_t d = DIVISORS[di];

        /* copy current polynomial: this implements the +1 term of θ₃(q^d) */
        for (size_t i = 0; i < len; ++i)
            new_res[i] = res[i];

        /* add the 2·q^{d·k²} terms */
        for (idx_t k = 1; ; ++k) {
            idx_t kk = k * k;
            idx_t t  = d * kk;
            if (t > n) break;

            seq_t add_mul = (seq_t)2;           /* multiplier 2 */
            for (size_t i = 0; i + t <= n; ++i) {
                /* new_res[i + t] += 2 * res[i] */
                new_res[i + t] += res[i] * add_mul;
            }
        }

        /* prepare for next divisor: swap pointers */
        seq_t *tmp = res;
        res = new_res;
        new_res = tmp;
    }

    seq_t answer = res[(size_t)n];

    free(res);
    free(new_res);
    return answer;
}
