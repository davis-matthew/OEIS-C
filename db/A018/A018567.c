/*
 * A018567  Divisors of 644.
 * https://oeis.org/A018567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 644: a(n) = the nth smallest divisor of 644 (1‑indexed) */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        size_t count = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {
            seq_t val2 = 1;
            for (int i = 0; i < e2; ++i) val2 = val2 * 2;
            for (int e7 = 0; e7 <= 1; ++e7) {
                seq_t val7 = val2;
                if (e7) val7 = val7 * 7;
                for (int e23 = 0; e23 <= 1; ++e23) {
                    seq_t val = val7;
                    if (e23) val = val * 23;
                    divisors[count++] = val;
                }
            }
        }
        /* simple bubble sort */
        for (size_t i = 0; i < 12; ++i) {
            for (size_t j = i + 1; j < 12; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        initialized = true;
    }

    if (n >= 1 && n <= 12u)
        return divisors[(size_t)(n - 1)];
    else
        return 0;
}
