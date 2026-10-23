/*
 * A018776  Divisors of 1015.
 * https://oeis.org/A018776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 1015 sorted in ascending order
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) = 1)  */
/* The sequence consists of all positive divisors of 1015 = 5·7·29. */

#include <stdbool.h>
#include <stddef.h>

/* Return the nth divisor of 1015 (1‑based).  For n > 8 the value 0 is
 * returned, as the sequence has only eight terms. */
seq_t a(idx_t n)
{
    static bool init_done = false;
    static seq_t divisors[8];

    if (!init_done) {
        /* Prime factors of 1015. */
        const seq_t primes[3] = {5, 7, 29};
        size_t cnt = 0;

        /* Generate all 2³ subsets of the primes. */
        for (idx_t mask = 0; mask < ((idx_t)1 << 3); ++mask) {
            seq_t prod = 1;
            if (mask & 1) prod *= primes[0];
            if (mask & 2) prod *= primes[1];
            if (mask & 4) prod *= primes[2];
            divisors[cnt++] = prod;
        }

        /* Simple insertion sort to get ascending order. */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        init_done = true;
    }

    if (n == 0 || n > 8) return 0;   /* out of bounds */
    return divisors[n - 1];
}
