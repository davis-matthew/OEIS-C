/*
 * A018329  Divisors of 195.
 * https://oeis.org/A018329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 195: a(1)=1, a(2)=3, a(3)=5, a(4)=13, a(5)=15, a(6)=39,
   a(7)=65, a(8)=195 */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t divisors[8];
    static size_t count = 0;

    if (!ready) {
        const uint64_t M = 195;
        seq_t small[8];
        seq_t large[8];
        size_t ns = 0, nl = 0;

        for (idx_t d = 1; (uint64_t)d * (uint64_t)d <= M; ++d) {
            if (M % d == 0) {
                small[ns++] = (seq_t)d;
                if ((uint64_t)d != M / d) {
                    large[nl++] = (seq_t)(M / d);
                }
            }
        }

        size_t idx = 0;
        for (size_t i = 0; i < ns; ++i) divisors[idx++] = small[i];
        for (size_t i = 0; i < nl; ++i) divisors[idx++] = large[nl - 1 - i];

        count = idx;   /* should be 8 */
        ready = true;
    }

    if (n == 0) return (seq_t)0;
    if ((idx_t)n <= count) return divisors[(size_t)(n - 1)];
    return (seq_t)0;
}
