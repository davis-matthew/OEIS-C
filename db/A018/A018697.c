/*
 * A018697  Divisors of 875.
 * https://oeis.org/A018697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 875 – a(n) is the n-th divisor of 875 in ascending order */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t divisors[8];
static bool divisors_init = false;

static void init_divisors(void)
{
    uint64_t pow5[4];
    uint64_t pow7[2];

    pow5[0] = (uint64_t)1;
    for (idx_t i = 1; i < 4; ++i)
        pow5[i] = pow5[i-1] * (uint64_t)5;

    pow7[0] = (uint64_t)1;
    pow7[1] = pow7[0] * (uint64_t)7;

    size_t idx = 0;
    for (idx_t a = 0; a <= 3; ++a)
        for (idx_t b = 0; b <= 1; ++b) {
            uint64_t val = pow5[a] * pow7[b];
            divisors[idx++] = (seq_t)val;
        }

    /* sort the array in ascending order (simple bubble sort – 8 elements) */
    for (size_t i = 0; i < 7; ++i)
        for (size_t j = 0; j < 7 - i; ++j)
            if (divisors[j] > divisors[j+1]) {
                seq_t tmp = divisors[j];
                divisors[j] = divisors[j+1];
                divisors[j+1] = tmp;
            }
}

seq_t a(idx_t n)
{
    if (!divisors_init) {
        init_divisors();
        divisors_init = true;
    }
    if (n >= 1 && n <= 8)
        return divisors[(size_t)(n-1)];
    return (seq_t)0;
}
