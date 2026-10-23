/*
 * A018317  Divisors of 172.
 * https://oeis.org/A018317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Enumerates the divisors of 172 in increasing order.
 * a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 43, a(5) = 86, a(6) = 172.
 * For n > 6 the function returns 0 as the sequence is finite.
 */
#include <stddef.h>
#include <stdlib.h>

static int cmp_seq_t(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t divs[16];   /* buffer large enough for all divisors */
    static size_t divcnt = 0;
    static int initialized = 0;

    if (!initialized) {
        const idx_t m = (idx_t)172;
        /* Compute all divisors of 172 */
        for (idx_t i = 1; i * i <= m; ++i) {
            if (m % i == 0) {
                divs[divcnt++] = (seq_t)i;
                if (i != m / i) {
                    divs[divcnt++] = (seq_t)(m / i);
                }
            }
        }
        /* Sort the divisors */
        qsort(divs, divcnt, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if (n == 0 || n > divcnt)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
