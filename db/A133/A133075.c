/*
 * A133075  Divisors of 2000.
 * https://oeis.org/A133075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 2000 (increasing order) */
#include <stddef.h>

/* a(n) = the n‑th divisor of 2000 in ascending order */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t divisors[20];
    static size_t divisor_count = 0;

    if (divisor_count == 0) {
        size_t pos = 0;
        for (int e2 = 0; e2 <= 4; ++e2) {            /* 2^e2, e2=0..4 */
            seq_t pow2 = 1;
            for (int k = 0; k < e2; ++k) pow2 *= 2;
            for (int e5 = 0; e5 <= 3; ++e5) {        /* 5^e5, e5=0..3 */
                seq_t pow5 = 1;
                for (int k = 0; k < e5; ++k) pow5 *= 5;
                divisors[pos++] = pow2 * pow5;
            }
        }
        /* insertion sort to get divisors in ascending order */
        for (size_t i = 1; i < pos; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        divisor_count = pos;
    }

    return (size_t)n <= divisor_count ? divisors[n - 1] : (seq_t)0;
}
