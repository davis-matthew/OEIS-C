/*
 * A018343  Divisors of 228.
 * https://oeis.org/A018343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th divisor of 228 by enumerating its prime factorization
   and precomputing the sorted list. */
#include <stddef.h>
#include <stdbool.h>

static seq_t divisors[12];
static bool initialized = false;

static void init_divisors(void)
{
    size_t idx = 0;
    /* 228 = 2^2 * 3^1 * 19^1 */
    static const seq_t p2[3] = {1, 2, 4};    /* 2^0, 2^1, 2^2 */
    static const seq_t p3[2] = {1, 3};       /* 3^0, 3^1 */
    static const seq_t p19[2] = {1, 19};     /* 19^0, 19^1 */

    for (size_t e2 = 0; e2 < 3; ++e2) {
        for (size_t e3 = 0; e3 < 2; ++e3) {
            for (size_t e19 = 0; e19 < 2; ++e19) {
                divisors[idx++] = p2[e2] * p3[e3] * p19[e19];
            }
        }
    }

    /* Simple insertion sort (12 elements only) */
    for (size_t i = 1; i < 12; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_divisors();
    }

    if (n < 1) {
        return (seq_t)0;
    }

    size_t pos = (size_t)n - 1;
    if (pos < 12) {
        return divisors[pos];
    }
    return (seq_t)0;
}
