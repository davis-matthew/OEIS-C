/*
 * A018407  Divisors of 351.
 * https://oeis.org/A018407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 351 in increasing order
   a(n) = the n-th divisor of 351 when arranged in ascending order
   (offset 1).  The divisors are 1, 3, 9, 13, 27, 39, 117, 351.   */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for qsort */

static seq_t divisors[8];
static int divisors_initialized = 0;

/* Helper: compare two seq_t values for qsort */
static int seq_compare(const void *p1, const void *p2)
{
    seq_t x = *(const seq_t *)p1;
    seq_t y = *(const seq_t *)p2;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

/* Build the array of divisors of 351 once */
static void init_divisors(void)
{
    if (divisors_initialized) return;

    size_t k = 0;
    /* 351 = 3^3 * 13^1, so all divisors are 3^a * 13^b, a=0..3, b=0..1 */
    for (idx_t a = 0; a <= 3; ++a) {
        seq_t pow3 = (seq_t)1;
        for (idx_t i = 0; i < a; ++i) pow3 *= 3;
        for (idx_t b = 0; b <= 1; ++b) {
            seq_t pow13 = (seq_t)1;
            for (idx_t j = 0; j < b; ++j) pow13 *= 13;
            divisors[k++] = pow3 * pow13;
        }
    }

    /* sort ascending */
    qsort(divisors, 8, sizeof(seq_t), seq_compare);
    divisors_initialized = 1;
}

seq_t a(idx_t n)
{
    init_divisors();

    if (n < 1 || n > 8) return 0;    /* undefined for out of range */
    return divisors[n - 1];
}
