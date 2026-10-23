/*
 * A018769  Divisors of 1002.
 * https://oeis.org/A018769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest divisor of 1002 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t divs[8];
    static bool ready = false;

    if (!ready) {
        const seq_t primes[3] = {(seq_t)2, (seq_t)3, (seq_t)167};
        size_t idx = 0;
        /* generate all products of subsets of primes */
        for (int mask = 0; mask < 8; ++mask) {
            seq_t prod = 1;
            if (mask & 1) prod *= primes[0];
            if (mask & 2) prod *= primes[1];
            if (mask & 4) prod *= primes[2];
            divs[idx++] = prod;
        }

        /* simple insertion sort to order the divisors */
        for (size_t i = 1; i < 8; ++i) {
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
        return (seq_t)0;            /* undefined for out‑of‑range indices */
    return divs[(size_t)(n - 1)];
}
