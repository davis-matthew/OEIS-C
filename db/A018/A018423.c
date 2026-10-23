/*
 * A018423  Divisors of 378.
 * https://oeis.org/A018423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 378 sorted in ascending order */
#include <stdlib.h>
#include <stdbool.h>

/* static array to hold the 16 divisors */
static seq_t divisors[16];
static bool initialized = false;

/* comparison function for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* generate all divisors of 378 and sort them */
static void init_divisors(void)
{
    if (initialized) return;

    /* 378 = 2^1 * 3^3 * 7^1 */
    const idx_t exp2 = 1, exp3 = 3, exp7 = 1;
    const seq_t p2 = (seq_t)2, p3 = (seq_t)3, p7 = (seq_t)7;

    idx_t pos = 0;
    for (idx_t i = 0; i <= exp2; ++i) {
        seq_t v2 = 1;
        for (idx_t k = 0; k < i; ++k) v2 *= p2;
        for (idx_t j = 0; j <= exp3; ++j) {
            seq_t v3 = v2;
            for (idx_t k = 0; k < j; ++k) v3 *= p3;
            for (idx_t k = 0; k <= exp7; ++k) {
                seq_t v7 = v3;
                for (idx_t kk = 0; kk < k; ++kk) v7 *= p7;
                divisors[pos++] = v7;
            }
        }
    }

    qsort(divisors, 16, sizeof(seq_t), cmp_seq_t);
    initialized = true;
}

/* OEIS A018423: a(n) = nth divisor of 378 (sorted ascending) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_divisors();
    if (n > 16) return 0;    /* no more divisors beyond 16th */
    return divisors[n - 1];
}
