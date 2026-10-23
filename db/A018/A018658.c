/*
 * A018658  Divisors of 808.
 * https://oeis.org/A018658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 808 in increasing order.
   a(n) returns the n‑th divisor (1 ≤ n ≤ 8);
   for n > 8 the function returns 0. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divs[8];
    static int ready = 0;

    if (!ready) {
        /* 808 = 2^3 * 101^1 */
        const seq_t p2 = (seq_t)2;
        const seq_t p101 = (seq_t)101;

        size_t pos = 0;
        /* generate all combinations of exponents 0..3 for 2 and 0..1 for 101 */
        for (uint64_t e2 = 0; e2 <= 3; ++e2) {
            seq_t val2 = (seq_t)1;
            for (uint64_t i = 0; i < e2; ++i)
                val2 *= p2;

            for (uint64_t e101 = 0; e101 <= 1; ++e101) {
                seq_t val101 = (seq_t)1;
                for (uint64_t j = 0; j < e101; ++j)
                    val101 *= p101;

                divs[pos++] = val2 * val101;
            }
        }

        /* insertion sort – array is tiny (8 elements) */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        ready = 1;
    }

    if (n >= 1 && (size_t)n <= 8)
        return divs[(size_t)n - 1];
    return (seq_t)0;
}
