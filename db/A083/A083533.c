/*
 * A083533  First difference sequence of A002202. Difference between
 *          consecutive possible values of phi(n), the Euler totient function
 *          A000010.
 * https://oeis.org/A083533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002202(n+1) - A002202(n), where A002202 is the * sorted
 *            list of distinct values of Euler's totient function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define MAX_N 2000000        /* maximum n for which φ(n) is precomputed */

/* Static data needed for lazy initialization. */
static unsigned char initialized = 0;
static seq_t *diffs = NULL;
static idx_t distinct_count = 0;

static void init(void)
{
    if (initialized) return;

    /* Allocate arrays for φ(i) and presence of φ(i). */
    uint32_t *phi = (uint32_t *)malloc((MAX_N + 1) * sizeof(uint32_t));
    uint8_t *seen = (uint8_t *)calloc(MAX_N + 1, sizeof(uint8_t));

    /* Compute φ(i) using the standard O(n log log n) sieve. */
    for (idx_t i = 0; i <= MAX_N; ++i) phi[i] = (uint32_t)i;
    for (idx_t p = 2; p <= MAX_N; ++p) {
        if (phi[p] == (uint32_t)p) {         /* p is prime */
            for (idx_t j = p; j <= MAX_N; j += p) {
                phi[j] -= phi[j] / (uint32_t)p;
            }
        }
    }

    /* Mark which φ values occur. */
    for (idx_t i = 1; i <= MAX_N; ++i) {
        uint32_t val = phi[i];
        seen[val] = 1;
    }

    /* Allocate array for differences of consecutive distinct φ values. */
    diffs = (seq_t *)malloc(MAX_N * sizeof(seq_t));  /* upper bound */

    /* Build the diff array. */
    int lastVal = -1;
    for (idx_t val = 1; val <= MAX_N; ++val) {
        if (seen[val]) {
            if (lastVal >= 0) {
                diffs[distinct_count] = (seq_t)(val - lastVal);
                ++distinct_count;
            }
            lastVal = (int)val;
        }
    }

    /* Clean up. */
    free(phi);
    free(seen);

    initialized = 1;
}

/* Public function defining the sequence A083533. */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0 || n > distinct_count) return (seq_t)0;
    return diffs[n - 1];
}
