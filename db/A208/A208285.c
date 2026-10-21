/*
 * A208285  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n > 7 * Initial values (OEIS OFFSET 1):
 *            a(1)=26, a(2)=676, a(3)=4374, a(4)=16872, a(5)=49130, *
 *            a(6)=119580, a(7)=256774
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 42, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined; return 0 */
        return 0;

    size_t idx = (size_t)n;    /* n ≥ 1 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Allocate and initialise base values on first call */
    if (cache == NULL) {
        cache_cap = 7;                           /* need indices 0…7 */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) abort();              /* OOM */
        cache[0] = 0;                            /* unused */
        cache[1] = 26;
        cache[2] = 676;
        cache[3] = 4374;
        cache[4] = 16872;
        cache[5] = 49130;
        cache[6] = 119580;
        cache[7] = 256774;
        cache_size = 7;
    }

    /* Grow cache if necessary to reach idx */
    if (idx > cache_cap) {
        size_t new_cap = idx;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    if (idx > cache_size) {
        for (size_t i = cache_size + 1; i <= idx; ++i) {
            seq_t s1 = cache[i-1];
            seq_t s2 = cache[i-2];
            seq_t s3 = cache[i-3];
            seq_t s4 = cache[i-4];
            seq_t s5 = cache[i-5];
            seq_t s6 = cache[i-6];
            seq_t s7 = cache[i-7];

            seq_t sum_pos = s1 * (seq_t)7 + s3 * (seq_t)35 + s5 * (seq_t)21 + s7;
            seq_t sum_neg = s2 * (seq_t)21 + s4 * (seq_t)35 + s6 * (seq_t)7;

            cache[i] = sum_pos - sum_neg;   /* guaranteed non‑negative */
        }
        cache_size = idx;
    }

    return cache[idx];
}
