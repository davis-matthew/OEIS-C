/*
 * A041976  Numerators of continued fraction convergents to sqrt(511).
 * https://oeis.org/A041976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   Let p[-2] = 0, p[-1] = 1 and let q_n be the nth partial quotient
 *            of the continued fraction expansion of √511: q_0 = 22, q_1 = 1,
 *            q_2 = 1, q_3 = 1, q_4 = 1, q_5 = 6, q_6 = 1, q_7 = 14, q_8 = 4,
 *            q_9 = 1, q_10 = 21, q_11 = 1, q_12 = 4, q_13 = 14, q_14 = 1,
 *            q_15 = 6, q_16 = 1, q_17 = 1, q_18 = 1, q_1...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t PERIOD[20] = { 1, 1, 1, 1, 6, 1, 14, 4,
                                  1, 21, 1, 4, 14, 1, 6, 1,
                                  1, 1, 1, 44 };
static const size_t PERIOD_LEN = 20;

static seq_t *cache = NULL;          /* stored numerators p_0 .. p_{last_computed} */
static size_t cache_capacity = 0;    /* allocated size of cache */
static size_t cache_filled  = 0;     /* number of elements currently computed */

seq_t a(idx_t n)
{
    /* Ensure cache exists and has enough capacity. */
    if (!cache) {
        /* Allocate an initial small capacity; will grow as needed. */
        cache_capacity = (n + 1 > 4) ? n + 1 : 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity : 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing values up to n. */
    if ((size_t)n + 1 > cache_filled) {
        for (size_t k = cache_filled; k <= (size_t)n; ++k) {
            seq_t qk;
            if (k == 0)
                qk = 22;                                   /* a_0 */
            else
                qk = PERIOD[(k - 1) % PERIOD_LEN];         /* q_k for k>=1 */

            seq_t pk;
            if (k == 0) {
                /* p_0 = q_0 * p_{-1} + p_{-2} = 22 * 1 + 0 = 22 */
                pk = qk;
            } else if (k == 1) {
                /* p_1 = q_1 * p_0 + p_{-1} = 1 * 22 + 1 = 23 */
                pk = qk * cache[0] + 1;
            } else {
                pk = qk * cache[k - 1] + cache[k - 2];
            }
            cache[k] = pk;
        }
        cache_filled = (size_t)n + 1;
    }

    return cache[n];
}
