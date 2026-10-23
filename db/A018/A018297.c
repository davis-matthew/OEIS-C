/*
 * A018297  Divisors of 132.
 * https://oeis.org/A018297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 132: a(n) is the n‑th divisor of 132 in ascending order */

#include <stddef.h>   /* size_t */

static seq_t divisors[12];
static idx_t divcnt = 0;

static void init_divisors(void)
{
    idx_t limit = (idx_t)132;
    seq_t tmp[64];
    idx_t cnt = 0;

    for (idx_t i = 1; i * i <= limit; ++i) {
        if (limit % i == 0) {
            tmp[cnt++] = (seq_t)i;
            if (i != limit / i)
                tmp[cnt++] = (seq_t)(limit / i);
        }
    }

    /* simple insertion sort for the small array */
    for (idx_t i = 1; i < cnt; ++i) {
        seq_t key = tmp[i];
        idx_t j = i;
        while (j > 0 && tmp[j - 1] > key) {
            tmp[j] = tmp[j - 1];
            --j;
        }
        tmp[j] = key;
    }

    for (idx_t i = 0; i < cnt; ++i)
        divisors[i] = tmp[i];
    divcnt = cnt;
}

seq_t a(idx_t n)
{
    if (divcnt == 0)
        init_divisors();

    if (n == 0 || n > divcnt)
        return (seq_t)0;
    return divisors[n - 1];
}
