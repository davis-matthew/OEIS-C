/*
 * A174903  Number of divisors d of n such that d<e<2*d for at least another
 *          divisor e of n.
 * https://oeis.org/A174903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d divisor of n | ∃e divisor of n: d<e<2*d }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static int cmp_idx_t(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    return (*x > *y) - (*x < *y);
}

seq_t a(idx_t n)
{
    /* collect divisors of n */
    /* For n up to 10 000 the number of divisors is well below 2048,
       so a static array is safe. */
    idx_t divs[2048];
    size_t cnt = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[cnt++] = i;
            if (i != n / i) {
                divs[cnt++] = n / i;
            }
        }
    }

    /* sort the divisors */
    qsort(divs, cnt, sizeof(idx_t), cmp_idx_t);

    /* count divisors that have another divisor in (d,2*d) */
    seq_t result = 0;
    for (size_t i = 0; i + 1 < cnt; ++i) {
        /* since divs are sorted, any divisor > divs[i] and < 2*divs[i]
           must be the next one, so we just check that pair. */
        if (divs[i + 1] < 2 * divs[i]) {
            ++result;
        }
    }

    return result;
}
