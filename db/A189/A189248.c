/*
 * A189248  Number of nX3 array permutations with each element making zero or
 *          one king moves.
 * https://oeis.org/A189248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) + 125*a(n-2) - 566*a(n-3) + 249*a(n-4) +
 *            916*a(n-5) - 1703*a(n-6) + 256*a(n-7) + 691*a(n-8) - 30*a(n-9) -
 *            9*a(n-10) Initial values: a(1)=3, a(2)=152, a(3)=2961,
 *            a(4)=76629, a(5)=1818336, a(6)=44308389, a(7)=1070959431,
 *            a(8)=25950094568, a(9)=628311228261, a(10)=15216392096673
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t SEEDS[11] = {
    0,                              /* unused 0 index */
    (seq_t)3,
    (seq_t)152,
    (seq_t)2961,
    (seq_t)76629,
    (seq_t)1818336,
    (seq_t)44308389,
    (seq_t)1070959431,
    (seq_t)25950094568ULL,
    (seq_t)628311228261ULL,
    (seq_t)15216392096673ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    if (n < 1)
        return (seq_t)0;

    /* allocate/resize cache if necessary */
    if (n > cached_max) {
        size_t new_size = (size_t)(n + 1);  /* indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;

        /* ensure indices up to cached_max are already filled */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = SEEDS[(size_t)i];
            } else {
                /* compute using the empirical recurrence */
                seq_t pos = ((seq_t)20   * cache[i-1]) +
                            ((seq_t)125  * cache[i-2]) +
                            ((seq_t)249  * cache[i-4]) +
                            ((seq_t)916  * cache[i-5]) +
                            ((seq_t)256  * cache[i-7]) +
                            ((seq_t)691  * cache[i-8]);

                seq_t neg = ((seq_t)566  * cache[i-3]) +
                            ((seq_t)1703 * cache[i-6]) +
                            ((seq_t)30   * cache[i-9]) +
                            ((seq_t)9    * cache[i-10]);

                cache[i] = pos - neg;   /* guaranteed non‑negative */
            }
        }
        cached_max = n;
    }
    return cache[n];
}
