/*
 * A018279  Divisors of 92.
 * https://oeis.org/A018279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 92 in increasing order (returns 0 for n
 *            larger than the number of divisors)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t divisors_of_92[6];
static bool divisors_initialized = false;

/* Helper: compute all divisors of 92 and store them sorted. */
static void init_divisors(void)
{
    seq_t temp[6];
    int idx = 0;
    for (int e2 = 0; e2 <= 2; ++e2)
    {
        seq_t pow2 = 1;
        for (int i = 0; i < e2; ++i)
            pow2 *= 2;
        for (int e23 = 0; e23 <= 1; ++e23)
        {
            seq_t pow23 = 1;
            for (int i = 0; i < e23; ++i)
                pow23 *= 23;
            temp[idx++] = pow2 * pow23;
        }
    }
    /* Sort the 6 values in increasing order. */
    for (int i = 0; i < 6; ++i)
        divisors_of_92[i] = temp[i];
    for (int i = 0; i < 6; ++i)
        for (int j = i + 1; j < 6; ++j)
            if (divisors_of_92[j] < divisors_of_92[i])
            {
                seq_t tmp = divisors_of_92[i];
                divisors_of_92[i] = divisors_of_92[j];
                divisors_of_92[j] = tmp;
            }
    divisors_initialized = true;
}

seq_t a(idx_t n)
{
    if (!divisors_initialized)
        init_divisors();

    if (n >= 1 && n <= 6)
        return divisors_of_92[(size_t)(n - 1)];
    return (seq_t)0;
}
