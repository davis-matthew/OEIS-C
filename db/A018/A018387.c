/*
 * A018387  Divisors of 312.
 * https://oeis.org/A018387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 312 in increasing order
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Comparator for qsort that works for any integral or big‑int seq_t */
static int compare_div(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[16];
    static const size_t div_count = 16;

    if (!initialized) {
        /* powers of the prime factors of 312 = 2^3 * 3 * 13 */
        seq_t pow2[4], pow3[2], pow13[2];
        pow2[0] = 1;  for (size_t i = 1; i < 4; ++i) pow2[i] = pow2[i - 1] * 2;
        pow3[0] = 1;  for (size_t i = 1; i < 2; ++i) pow3[i] = pow3[i - 1] * 3;
        pow13[0] = 1; for (size_t i = 1; i < 2; ++i) pow13[i] = pow13[i - 1] * 13;

        size_t idx = 0;
        for (size_t i = 0; i < 4; ++i)        /* exponent of 2 */
            for (size_t j = 0; j < 2; ++j)    /* exponent of 3 */
                for (size_t k = 0; k < 2; ++k)/* exponent of 13 */
                    divisors[idx++] = pow2[i] * pow3[j] * pow13[k];

        qsort(divisors, div_count, sizeof(seq_t), compare_div);
        initialized = true;
    }

    if (n >= 1 && (n - 1) < div_count)
        return divisors[n - 1];
    return (seq_t)0;
}
