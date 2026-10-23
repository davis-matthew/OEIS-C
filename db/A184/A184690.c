/*
 * A184690  1/3 the number of nX5 0..2 arrays with no element equal both to
 *          the element above and to the element to its left.
 * https://oeis.org/A184690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence * a(1) = 81 * a(2) = 12288 * a(3) = 1841856
 *            * a(4) = 275895264 * a(5) = 41325395328 * a(6) = 6189975781728 *
 *            a(7) = 927173138866560 * a(8) = 138877768818516672 * a(n) =
 *            171*a(n‑1) - 3370*a(n‑2) + 29805*a(n‑3) - 149108*a(n‑4) * +
 *            447784*a(n‑5) - 778496*a(n‑6) + 649216*a(n‑7) -...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cached = NULL;
    static idx_t cap = 0;
    static idx_t computed = 0;

    const seq_t init[8] = {
        81ULL,
        12288ULL,
        1841856ULL,
        275895264ULL,
        41325395328ULL,
        6189975781728ULL,
        927173138866560ULL,
        138877768818516672ULL
    };

    /* Return cached value if already computed */
    if (n <= computed && n > 0) {
        return cached[n - 1];
    }

    /* Ensure cache capacity */
    if (cached == NULL || cap < n) {
        cached = (seq_t *)realloc(cached, n * sizeof(seq_t));
        cap = n;
    }

    /* Fill missing initial values up to 8 */
    if (computed < 8) {
        idx_t up = (n < 8) ? n : 8;
        for (idx_t i = computed; i < up; ++i) {
            cached[i] = init[i];
        }
        computed = up;
        if (n <= computed) {
            return cached[n - 1];
        }
    }

    /* Compute remaining terms by recurrence */
    for (idx_t i = computed; i < n; ++i) {
        seq_t val = cached[i - 1] * 171ULL;
        val += cached[i - 3] * 29805ULL;
        val += cached[i - 5] * 447784ULL;
        val += cached[i - 7] * 649216ULL;
        val -= cached[i - 2] * 3370ULL;
        val -= cached[i - 4] * 149108ULL;
        val -= cached[i - 6] * 778496ULL;
        val -= cached[i - 8] * 114688ULL;
        cached[i] = val;
    }

    computed = n;
    return cached[n - 1];
}
