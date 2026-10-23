/*
 * A018365  Divisors of 270.
 * https://oeis.org/A018365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 270 (sorted ascending)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: comparison function for qsort on seq_t */
static int cmp_seqt(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Static storage for the 16 divisors of 270 */
static seq_t divisors[16];
static size_t divisor_count = 0;
static bool initialized = false;

/* Initialise the divisor list and sort it */
static void init_divisors(void)
{
    if (initialized) return;

    /* 270 = 2^1 * 3^3 * 5^1 */
    seq_t pow2[2] = { (seq_t)1, (seq_t)2 };
    seq_t pow3[4] = { (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)27 };
    seq_t pow5[2] = { (seq_t)1, (seq_t)5 };

    size_t pos = 0;
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 4; ++j)
            for (size_t k = 0; k < 2; ++k)
                divisors[pos++] = pow2[i] * pow3[j] * pow5[k];

    divisor_count = pos;
    qsort(divisors, divisor_count, sizeof(seq_t), cmp_seqt);
    initialized = true;
}

/* Seq_t a(idx_t n) ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_divisors();

    /* OEIS offset: a(1) corresponds to divisors[0] */
    if (n == 0) return (seq_t)0;
    if ((idx_t)(n - 1) >= divisor_count) return (seq_t)0;
    return divisors[n - 1];
}
