/*
 * A018628  Divisors of 754.
 * https://oeis.org/A018628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018628: Divisors of 754 in ascending order */
/* For n = 1 … 8, a(n) is the n‑th divisor of 754.  For larger n the
   sequence is undefined, so the function returns 0. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divs[8];
    static int initialized = 0;

    if (!initialized) {
        /* 754 = 2 * 13 * 29 */
        const uint64_t primes[3] = {2ULL, 13ULL, 29ULL};
        int exp[3] = {1, 1, 1};
        size_t idx = 0;

        for (int i = 0; i <= exp[0]; ++i)
            for (int j = 0; j <= exp[1]; ++j)
                for (int k = 0; k <= exp[2]; ++k) {
                    seq_t d = 1;
                    if (i) d *= primes[0];
                    if (j) d *= primes[1];
                    if (k) d *= primes[2];
                    divs[idx++] = d;
                }

        /* Small bubble sort for the 8 elements */
        for (size_t i = 0; i < 7; ++i)
            for (size_t j = i + 1; j < 8; ++j)
                if (divs[i] > divs[j]) {
                    seq_t temp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = temp;
                }

        initialized = 1;
    }

    return (n >= 1 && n <= 8) ? divs[n - 1] : 0;
}
