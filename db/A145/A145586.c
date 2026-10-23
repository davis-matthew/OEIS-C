/*
 * A145586  a(n) = number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^8.
 * https://oeis.org/A145586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of integers removed by the nth prime in
 *            Eratosthenes's sieve for N = 256 (2^8). This computes the
 *            sequence by performing a sieve and recording the counts of newly
 *            marked composites at each prime step
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static data, initialized on first call */
    static bool initialized = false;
    static seq_t result[61] = {0};          /* indices 1..54 used */
    static bool composite[257] = {0};

    if (!initialized) {
        const idx_t N = 256;           /* fixed limit for this sequence */
        idx_t prime_idx = 0;

        for (idx_t p = 2; p <= N; ++p) {
            if (!composite[p]) {       /* p is a prime */
                ++prime_idx;           /* next result index */
                result[prime_idx] = 0; /* reset count */

                for (idx_t m = p * 2; m <= N; m += p) {
                    if (!composite[m]) {
                        result[prime_idx] += 1;
                        composite[m] = true;  /* mark as removed */
                    }
                }
            }
        }
        initialized = true;
    }

    /* index 0 is outside the offset; return 0 for safety */
    if (n == 0 || n >= sizeof(result)/sizeof(result[0]))
        return 0;
    return result[n];
}
