/*
 * A018425  Divisors of 384.
 * https://oeis.org/A018425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 384 in ascending order
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* static cache of all divisors of 384 */
static seq_t divisors[16];
static bool initialized = false;

/* initialize the divisor array and sort it */
static void init_divisors(void)
{
    idx_t i, k = 0;

    /* generate divisors of the form 2^i (i = 0..7) */
    for (i = 0; i <= 7; ++i)
        divisors[k++] = (seq_t)(1 << i);

    /* generate divisors of the form 2^i * 3 (i = 0..7) */
    for (i = 0; i <= 7; ++i)
        divisors[k++] = (seq_t)((1 << i) * 3);

    /* simple insertion sort (16 elements is trivial) */
    for (i = 0; i < 16; ++i)
        for (idx_t j = i + 1; j < 16; ++j)
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    if (!initialized) {
        init_divisors();
        initialized = true;
    }

    if (n > 16)
        return 0;                 /* out of range – no such divisor */

    return divisors[n - 1];
}
