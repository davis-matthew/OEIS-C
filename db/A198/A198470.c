/*
 * A198470  Numbers that are larger than the sum of their deficient divisors.
 * https://oeis.org/A198470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that is larger than the sum of its
 *            deficient divisors
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* external linkage requested by the problem statement */
seq_t a(idx_t n) {
    /* static initialization --- runs at first call only */
    static bool initialized = false;
    /* large enough constant to cover 10 000 terms comfortably */
    const idx_t LIMIT = 2000000;

    /* internal arrays for sieve */
    static uint64_t *sigma = NULL;
    static uint8_t  *def = NULL;
    static uint64_t *sumDef = NULL;

    /* vector of sequence values */
    static seq_t *seqVals = NULL;
    static size_t seqCnt = 0;

    if (!initialized) {
        /* allocate arrays */
        sigma = (uint64_t*)malloc(sizeof(uint64_t) * (LIMIT + 1));
        def  = (uint8_t*)   malloc(sizeof(uint8_t)  * (LIMIT + 1));
        sumDef = (uint64_t*)malloc(sizeof(uint64_t) * (LIMIT + 1));
        seqVals = (seq_t*)malloc(sizeof(seq_t) * LIMIT);   /* gross over-estimate */

        /* compute sigma(m) = sum of divisors of m */
        for (idx_t i = 1; i <= LIMIT; ++i) {
            for (idx_t j = i; j <= LIMIT; j += i) {
                sigma[j] += i;
            }
        }

        /* mark deficient numbers: sigma(d) < 2*d */
        for (idx_t i = 1; i <= LIMIT; ++i) {
            def[i] = sigma[i] < (uint64_t)2 * (uint64_t)i;
        }

        /* sumDef[n] = sum of deficient divisors of n */
        for (idx_t d = 1; d <= LIMIT; ++d) {
            if (!def[d]) continue;
            for (idx_t m = d; m <= LIMIT; m += d) {
                sumDef[m] += d;
            }
        }

        /* collect sequence terms */
        seqCnt = 0;
        for (idx_t m = 1; m <= LIMIT; ++m) {
            if ((uint64_t)m > sumDef[m]) {
                seqVals[seqCnt++] = (seq_t)m;
            }
        }

        initialized = true;
    }

    /* 1‑based indexing: a(1) is first term in seqVals */
    if (n == 0) return 0;      /* undefined, but safe fallback */
    if (n > seqCnt) return 0;  /* out of pre‑computed range */
    return seqVals[n - 1];
}
