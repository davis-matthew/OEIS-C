/*
 * A018260  Divisors of 45.
 * https://oeis.org/A018260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 45 in increasing order
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t divisors[6];
static bool initialized = false;

static void compute_divisors(void)
{
    idx_t idx = 0;
    seq_t pow3 = (seq_t)1;
    for (int i = 0; i <= 2; ++i) {
        seq_t pow5 = (seq_t)1;
        for (int j = 0; j <= 1; ++j) {
            divisors[idx++] = pow3 * pow5;
            pow5 *= (seq_t)5;
        }
        pow3 *= (seq_t)3;
    }
    /* bubble sort */
    for (idx_t i = 0; i < 6; ++i) {
        for (idx_t j = i + 1; j < 6; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_divisors();
        initialized = true;
    }
    if (n >= 1 && n <= 6) {
        return divisors[n - 1];
    }
    return (seq_t)0;
}
