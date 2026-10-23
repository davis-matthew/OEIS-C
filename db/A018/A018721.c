/*
 * A018721  Divisors of 918.
 * https://oeis.org/A018721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: All divisors of 918 sorted ascending */
#include <stdlib.h>
#include <stdbool.h>

static seq_t divisors[16];
static bool initialized = false;

static int cmp_seq(const void *a, const void *b)
{
    const seq_t va = *(const seq_t *)a;
    const seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void init_divisors(void)
{
    const idx_t exp2_max = 1;
    const idx_t exp3_max = 3;
    const idx_t exp17_max = 1;
    idx_t idx = 0;
    for (idx_t e2 = 0; e2 <= exp2_max; ++e2)
        for (idx_t e3 = 0; e3 <= exp3_max; ++e3)
            for (idx_t e17 = 0; e17 <= exp17_max; ++e17)
            {
                seq_t val = 1;
                for (idx_t i = 0; i < e2; ++i) val *= (seq_t)2;
                for (idx_t i = 0; i < e3; ++i) val *= (seq_t)3;
                for (idx_t i = 0; i < e17; ++i) val *= (seq_t)17;
                divisors[idx++] = val;
            }
    qsort(divisors, 16, sizeof(seq_t), cmp_seq);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_divisors();
    if (n == 0 || n > 16) return 0;
    return divisors[n-1];
}
