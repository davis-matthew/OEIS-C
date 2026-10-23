/*
 * A195212  Numbers n for which the exponent of the largest prime factor of n
 *          is even.
 * https://oeis.org/A195212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute the sequence of numbers n such that the exponent of the
 *            largest prime factor of n is even. The sequence starts at n = 4
 *            (a(1) = 4). The function generates terms on demand and memoises
 *            them
 * Range:     u8 n <= 25, u16 n <= 1213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n) {
    static idx_t next_m = 1;          /* next integer to test */
    static size_t terms = 0;          /* number of terms computed */
    static seq_t *seq = NULL;         /* memoised sequence values */
    static size_t capacity = 0;       /* allocated capacity of seq */

    /* Generate terms until we have at least n of them. */
    while (terms < n) {
        uint64_t m = (uint64_t)next_m;

        if (m >= 2) {
            uint64_t p = largest_prime_factor(m);
            uint64_t exp = 0;
            uint64_t tmp = m;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            /* include m if the exponent of its largest prime factor is even */
            if ((exp & 1ULL) == 0) {
                if (terms == capacity) {
                    size_t new_cap = capacity ? capacity * 2 : 4;
                    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    if (!new_seq) abort();
                    seq = new_seq;
                    capacity = new_cap;
                }
                seq[terms++] = (seq_t)m;
            }
        }

        next_m++;
    }

    return seq[n - 1];
}
