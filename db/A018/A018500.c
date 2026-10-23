/*
 * A018500  Divisors of 524.
 * https://oeis.org/A018500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 524: precomputed finite list (1,2,4,131,262,524) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache the divisors of 524 the first time the function is called */
    static bool ready = false;
    static seq_t d[16];   /* enough slots for all divisors of 524 (6 total) */
    static idx_t cnt = 0;

    if (!ready) {
        const uint64_t N = 524ULL;  /* 524 = 2^2 * 131 */
        idx_t i, j = 0;

        /* collect all divisors */
        for (i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                d[j++] = (seq_t)i;
                if (i != N / i) {
                    d[j++] = (seq_t)(N / i);
                }
            }
        }

        /* simple bubble sort to order the divisors ascending */
        for (i = 0; i < j; ++i) {
            idx_t k;
            for (k = i + 1; k < j; ++k) {
                if (d[i] > d[k]) {
                    seq_t tmp = d[i];
                    d[i] = d[k];
                    d[k] = tmp;
                }
            }
        }

        cnt = j;
        ready = true;
    }

    /* Return the divisor if the index is within bounds, otherwise 0 */
    if (n >= 1 && n <= cnt) {
        return d[n - 1];
    } else {
        return (seq_t)0;
    }
}
