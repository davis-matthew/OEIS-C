/*
 * A007774  Numbers that are divisible by exactly 2 different primes; numbers
 *          n with omega(n) = A001221(n) = 2.
 * https://oeis.org/A007774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: a(n) = n-th number m with omega(m) = 2 (sieve omega up to a limit, doubling as needed) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *terms = NULL;
static uint64_t nterms = 0;

static void build(uint64_t limit) {
    unsigned char *om = (unsigned char *)calloc((size_t)(limit + 1), 1);
    uint64_t p, m, cnt = 0;
    for (p = 2; p <= limit; p++) {
        if (om[p] == 0) {
            for (m = p; m <= limit; m += p) {
                if (om[m] < 255) om[m]++;
            }
        }
    }
    for (m = 2; m <= limit; m++) if (om[m] == 2) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    nterms = 0;
    for (m = 2; m <= limit; m++) if (om[m] == 2) terms[nterms++] = m;
    free(om);
}

seq_t a(idx_t n) {
    if (n > nterms) {
        uint64_t limit = 64;
        while (true) {
            /* a(n) ~ n log n / log log n; grow until enough terms */
            if (limit >= n * 4) {
                build(limit);
                if (nterms >= n) break;
            }
            limit *= 2;
        }
    }
    return (seq_t)terms[n - 1];
}
