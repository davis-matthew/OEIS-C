/*
 * A018750  Divisors of 969.
 * https://oeis.org/A018750
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

/* Divisors of 969: a(n) = n‑th divisor of 969 in increasing order */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Prime factorisation of 969 is 3 * 17 * 19 */
    static seq_t divs[8];
    static bool ready = false;

    if (!ready) {
        const idx_t primes[] = {3, 17, 19};
        const size_t np = sizeof(primes) / sizeof(primes[0]);
        const size_t total = (size_t)1 << np;   /* 2ⁿ = 8 */

        /* Enumerate all products of subsets of the primes */
        size_t i = 0;
        for (idx_t mask = 0; mask < (idx_t)1 << np; ++mask) {
            seq_t prod = (seq_t)1;
            for (size_t j = 0; j < np; ++j)
                if (mask & ((idx_t)1 << j))
                    prod = prod * (seq_t)primes[j];
            divs[i++] = prod;
        }

        /* Sort the divisor list in ascending order (insertion sort) */
        for (size_t i = 1; i < total; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        ready = true;
    }

    if (n == 0 || n > 8)
        return (seq_t)0;   /* out of range */
    return divs[n - 1];
}
