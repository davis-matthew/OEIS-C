/*
 * A173576  6-Factorions: equal to the sum of the 6-ple factorials of their
 *          digits in base 10.
 * https://oeis.org/A173576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 6-factorions: numbers equal to the sum of the 6-ple factorials (multifactorials with step 6) of their decimal digits. */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of all 6-factorions found (up to 999). */
    static seq_t cache[1000];
    static idx_t cache_len = 0;
    static bool ready = false;

    if (!ready) {
        /* Mapping of a single digit d -> d!^(6). */
        static const seq_t dF[10] = {0, 1, 2, 3, 4, 5, 6, 7, 16, 27};

        for (idx_t i = 1; i < 1000; ++i) {
            idx_t x = i;
            seq_t sum = 0;
            while (x > 0) {
                unsigned d = (unsigned)(x % 10);
                sum += dF[d];
                x /= 10;
            }
            if (sum == i) {
                cache[cache_len++] = i;
            }
        }
        ready = true;
    }

    if (n == 0) return 0;          /* below offset – undefined, return 0 */
    if (n <= cache_len) return cache[n - 1];

    return 0;                      /* no value defined beyond known terms */
}
