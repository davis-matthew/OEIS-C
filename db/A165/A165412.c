/*
 * A165412  Divisors of 2520.
 * https://oeis.org/A165412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = n-th divisor of 2520 in ascending order
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t divisors[48];
static bool initialized = false;

static void init_divisors(void)
{
    if (initialized) return;

    idx_t count = 0;
    /* Generate all 48 divisors (2^e2 * 3^e3 * 5^e5 * 7^e7) */
    for (idx_t e2 = 0; e2 <= 3; ++e2) {
        seq_t v2 = 1;
        for (idx_t i = 0; i < e2; ++i) v2 *= 2;
        for (idx_t e3 = 0; e3 <= 2; ++e3) {
            seq_t v3 = v2;
            for (idx_t i = 0; i < e3; ++i) v3 *= 3;
            for (idx_t e5 = 0; e5 <= 1; ++e5) {
                seq_t v5 = v3;
                if (e5) v5 *= 5;
                for (idx_t e7 = 0; e7 <= 1; ++e7) {
                    seq_t v7 = v5;
                    if (e7) v7 *= 7;
                    divisors[count++] = v7;
                }
            }
        }
    }

    /* Insertion sort the 48 numbers */
    for (size_t i = 1; i < 48; ++i) {
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
    if (n < 1) return 0;          /* undefined for n < 1 */
    init_divisors();
    if (n > 48) return 0;         /* sequence only defined for n ≤ 48 */
    return divisors[(size_t)(n - 1)];
}
