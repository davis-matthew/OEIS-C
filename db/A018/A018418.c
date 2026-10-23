/*
 * A018418  Divisors of 370.
 * https://oeis.org/A018418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 370, sorted increasingly
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* extern linkage as required */
seq_t a(idx_t n) {
    /* prime factors of 370: 2, 5, 37 (each exponent 1) */
    static seq_t *divs = NULL;
    static size_t divcnt = 0;

    if (!divs) {
        /* number of divisors = 2^3 = 8 */
        const uint64_t primes[3] = {2, 5, 37};
        size_t p = 3;
        divcnt = (size_t)1ULL << p;                 /* 8 */
        divs = (seq_t *)malloc(divcnt * sizeof(seq_t));
        if (!divs) return 0;                       /* allocation failure */

        /* generate all products of subsets of primes */
        divs[0] = 1;
        size_t idx = 1;
        for (size_t i = 0; i < p; ++i) {
            uint64_t prime = primes[i];
            size_t prev = idx;
            for (size_t j = 0; j < prev; ++j) {
                divs[idx++] = divs[j] * prime;
            }
        }

        /* sort divisors ascending (simple insertion sort, tiny array) */
        for (size_t i = 1; i < divcnt; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
    }

    if (n == 0) return 0;
    if ((idx_t)(n - 1) >= divcnt) return 0;
    return divs[n - 1];
}
