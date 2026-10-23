/*
 * A018752  Divisors of 972.
 * https://oeis.org/A018752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 972 (sorted ascending)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The only divisors of 972 are 2^a * 3^b with 0 ≤ a ≤ 2, 0 ≤ b ≤ 5 */
    if (n == 0) return 0;   /* undefined for n < 1, return 0 for safety */

    /* All 18 divisors of 972 */
    seq_t divisors[18];

    /* Precompute powers of 2 and 3 */
    seq_t pow2[3] = {1, 2, 4};           /* 2^0, 2^1, 2^2 */
    seq_t pow3[6] = {1, 3, 9, 27, 81, 243}; /* 3^0 .. 3^5 */

    size_t pos = 0;
    for (idx_t i = 0; i <= 2; ++i) {
        for (idx_t j = 0; j <= 5; ++j) {
            divisors[pos++] = pow2[i] * pow3[j];
        }
    }

    /* Sort ascending (simple insertion sort, works for 18 elements) */
    for (size_t i = 1; i < 18; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    if (n > 18) return 0;  /* beyond defined range */
    return divisors[n - 1];
}
