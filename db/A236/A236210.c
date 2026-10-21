/*
 * A236210  Pairs of "harmonic numbers" 2^m * 3^n that differ by 1.
 * https://oeis.org/A236210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236210: Harmonic numbers 2^m * 3^n that differ by 1.
   The returned sequence contains each such number once per pair it
   belongs to; e.g. 2 appears twice because it is in pairs (1,2) and (2,3). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparison function for qsort using the operators of seq_t. */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Maximum exponents used to generate all harmonic numbers needed
   for the pairs of A236210.  The only pairs are (1,2), (2,3), (3,4)
   and (8,9); thus exponents up to 10 are more than sufficient. */
#define MAX_M 10
#define MAX_N 10

/* Static storage for the pre‑computed output sequence. */
static seq_t output[256];
static size_t output_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Initialisation runs only once. */
    if (!initialized) {
        /* Powers of 2 and 3. */
        seq_t pow2[MAX_M + 1];
        seq_t pow3[MAX_N + 1];
        pow2[0] = 1;
        pow3[0] = 1;
        for (int m = 1; m <= MAX_M; ++m)
            pow2[m] = pow2[m-1] * 2;
        for (int p = 1; p <= MAX_N; ++p)
            pow3[p] = pow3[p-1] * 3;

        /* Gather all harmonic numbers 2^m*3^n into a small array. */
        seq_t harmonic[121];
        size_t hcnt = 0;
        for (int m = 0; m <= MAX_M; ++m) {
            for (int p = 0; p <= MAX_N; ++p) {
                harmonic[hcnt++] = pow2[m] * pow3[p];
            }
        }

        /* Sort the harmonic numbers. */
        qsort(harmonic, hcnt, sizeof(seq_t), seq_compare);

        /* Search for consecutive pairs and collect the required output. */
        output_len = 0;
        for (size_t i = 0; i + 1 < hcnt; ++i) {
            if (harmonic[i+1] == harmonic[i] + 1) {
                output[output_len++] = harmonic[i];
                output[output_len++] = harmonic[i+1];
            }
        }

        initialized = true;
    }

    /* Return the requested element or 0 if n is out of range. */
    if (n >= 1 && (idx_t)n <= (idx_t)output_len)
        return output[n-1];
    else
        return 0;
}
