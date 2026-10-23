/*
 * A018391  Divisors of 320.
 * https://oeis.org/A018391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 320 in ascending order
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t divisors[14];
static bool initialized = false;

static int seqcmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static void init_divisors(void)
{
    if (initialized) return;
    size_t i = 0;
    const seq_t one = (seq_t)1;
    for (idx_t a = 0; a <= 6; ++a) {
        seq_t pow2 = one << a;                     /* 2^a */
        divisors[i++] = pow2;                      /* 5^0 */
        divisors[i++] = pow2 * (seq_t)5;           /* 5^1 */
    }
    qsort(divisors, 14, sizeof(seq_t), seqcmp);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* out of range */
    init_divisors();
    if (n <= 14) return divisors[n - 1];
    return (seq_t)0;                              /* sequence ends after 14 terms */
}
