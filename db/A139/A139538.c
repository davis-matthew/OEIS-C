/*
 * A139538  Odd numbers of the form x^2+5y^2 not divisible by 5, with both x
 *          and y >== 1.
 * https://oeis.org/A139538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd number representable as x^2 + 5*y^2 with
 *            integers x, y ≥ 1 and not divisible by 5. Computation is done by
 *            generating all such values up to a dynamic upper bound,
 *            deduplicating and sorting them. The sequence is memoised so
 *            subsequent calls are fast, independent of the call order
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)           /* undefined, but return 0 gracefully */
        return 0;

    /* generate more terms if required */
    if (n > cache_size) {
        /* start with a bound roughly proportional to n and grow as needed */
        idx_t limit = (idx_t)(n * 10);
        while (1) {
            /* boolean mask to mark which values appear */
            size_t sz = (size_t)(limit + 1);
            bool *present = (bool *)calloc(sz, 1);
            if (!present)
                abort();

            /* enumerate x, y pairs generating values ≤ limit */
            for (uint64_t x = 1; ; ++x) {
                seq_t xsq = (seq_t)x * (seq_t)x;
                if (xsq > (seq_t)limit)
                    break;
                for (uint64_t y = 1; ; ++y) {
                    seq_t ysq = (seq_t)y * (seq_t)y;
                    seq_t val = xsq + (seq_t)5 * ysq;
                    if (val > (seq_t)limit)
                        break;
                    /* odd and not divisible by 5 */
                    if ((val & 1) && (val % 5 != 0)) {
                        idx_t idx = (idx_t)val;
                        present[idx] = true;
                    }
                }
            }

            /* count unique values */
            idx_t count = 0;
            for (size_t v = 1; v <= limit; ++v)
                if (present[v])
                    ++count;

            /* if we have enough terms, build the sorted list */
            if (count >= n) {
                seq_t *new_cache = (seq_t *)malloc(count * sizeof(seq_t));
                if (!new_cache)
                    abort();

                idx_t pos = 0;
                for (size_t v = 1; v <= limit; ++v) {
                    if (present[v])
                        new_cache[pos++] = (seq_t)v;
                }

                free(present);
                free(cache);
                cache     = new_cache;
                cache_size = count;
                break;
            }

            free(present);
            /* increase bound and try again */
            limit <<= 1;
        }
    }

    return cache[n - 1];
}
