/*
 * A018379  Divisors of 294.
 * https://oeis.org/A018379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 294 (2 * 3 * 7^2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* precompute sorted list of divisors of 294 */
    static seq_t divisors[12];
    static int initialized = 0;
    if (!initialized) {
        idx_t pos = 0;
        /* exponents ranges: 2^0..1, 3^0..1, 7^0..2 */
        for (idx_t e0 = 0; e0 <= 1; ++e0) {
            seq_t part0 = 1;
            for (idx_t i = 0; i < e0; ++i) part0 *= 2;
            for (idx_t e1 = 0; e1 <= 1; ++e1) {
                seq_t part1 = part0;
                for (idx_t i = 0; i < e1; ++i) part1 *= 3;
                for (idx_t e2 = 0; e2 <= 2; ++e2) {
                    seq_t val = part1;
                    for (idx_t i = 0; i < e2; ++i) val *= 7;
                    divisors[pos++] = val;
                }
            }
        }
        /* simple bubble sort */
        for (idx_t i = 0; i < 12; ++i)
            for (idx_t j = i + 1; j < 12; ++j)
                if (divisors[j] < divisors[i]) {
                    seq_t t = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = t;
                }
        initialized = 1;
    }
    if (n >= 1 && n <= 12) {
        return divisors[n - 1];
    }
    /* sequence is finite; return 0 for indices beyond the last divisor */
    return 0;
}
