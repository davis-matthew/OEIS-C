/*
 * A027899  Divisors of 10^11 + 1.
 * https://oeis.org/A027899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 100000000001 (OEIS A027899)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_cmp.h"

static seq_t *divs = NULL;
static size_t divs_count = 0;
static bool init_done = false;

static void init_divisors(void)
{
    if (init_done) return;
    const idx_t N = 100000000001ULL;

    /* Count divisors */
    size_t count = 0;
    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            idx_t j = N / i;
            count += (i == j) ? 1 : 2;
        }
    }

    /* Allocate exactly enough space */
    divs = (seq_t *)malloc(count * sizeof(seq_t));
    size_t pos = 0;

    /* Collect divisors */
    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            idx_t j = N / i;
            if (i == j) {
                divs[pos++] = (seq_t)i;
            } else {
                divs[pos++] = (seq_t)i;
                divs[pos++] = (seq_t)j;
            }
        }
    }

    /* Sort ascending */
    qsort(divs, count, sizeof(seq_t), seq_cmp);
    divs_count = count;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) init_divisors();
    if (n == 0 || (size_t)n > divs_count) return (seq_t)0;
    return divs[n - 1];
}
