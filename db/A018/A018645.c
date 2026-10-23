/*
 * A018645  Divisors of 784.
 * https://oeis.org/A018645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 784 (sorted increasingly)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: comparison function for qsort on seq_t types */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Return the n-th divisor of 784, sorted increasingly.
 * If n is out of bounds (n < 1 or n > 15), 0 is returned. */
seq_t a(idx_t n)
{
    /* 784 = 2^4 * 7^2, 5 * 3 = 15 divisors */
    const idx_t exp2 = 4;
    const idx_t exp7 = 2;
    const size_t total = 15;
    static seq_t divisors[15];
    static int initialized = 0;

    if (!initialized) {
        size_t pos = 0;
        for (idx_t i = 0; i <= exp2; ++i) {
            /* compute 2^i */
            seq_t pow2 = 1;
            for (idx_t k = 0; k < i; ++k) {
                pow2 *= 2;
            }
            for (idx_t j = 0; j <= exp7; ++j) {
                /* compute 7^j */
                seq_t pow7 = 1;
                for (idx_t k = 0; k < j; ++k) {
                    pow7 *= 7;
                }
                /* store product */
                divisors[pos++] = pow2 * pow7;
            }
        }
        /* sort the array */
        qsort(divisors, total, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if (n < 1 || n > total) {
        return (seq_t)0;
    }
    return divisors[(size_t)(n - 1)];
}
