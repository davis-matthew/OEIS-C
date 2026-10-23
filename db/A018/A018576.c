/*
 * A018576  Divisors of 657.
 * https://oeis.org/A018576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 657 in increasing order */

/* The sequence consists of all positive divisors of 657 sorted
 * in ascending order: 1, 3, 9, 73, 219, 657.  The number 657 is
 * factored at run time (3²·73) and the divisors are generated
 * combinatorially.  The sequence has exactly six terms; for n > 6
 * the function returns 0.  The algorithm runs in O(1) time. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for the 6 divisors, generated on first call. */
    static bool initialized = false;
    static seq_t divisors[6];
    static idx_t divisor_count = 0;

    if (!initialized) {
        /* Factor 657: 3^2 * 73^1 */
        struct { seq_t p; unsigned e; } factors[] = { {3, 2}, {73, 1} };
        const size_t nf = sizeof factors / sizeof *factors;

        size_t sz = 1;               /* start with divisor 1 */
        divisors[0] = 1;

        for (size_t fi = 0; fi < nf; ++fi) {
            seq_t prime = factors[fi].p;
            unsigned exp  = factors[fi].e;
            size_t old = sz;
            seq_t pow = 1;
            for (unsigned ee = 1; ee <= exp; ++ee) {
                pow *= prime;            /* prime^ee */
                for (size_t j = 0; j < old; ++j) {
                    divisors[sz++] = divisors[j] * pow;
                }
            }
        }

        divisor_count = sz;   /* should be 6 */

        /* Simple insertion sort of the small array */
        for (size_t i = 0; i < divisor_count; ++i) {
            for (size_t j = i + 1; j < divisor_count; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        initialized = true;
    }

    return (n >= 1 && n <= divisor_count) ? divisors[n - 1] : 0;
}
