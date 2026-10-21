/*
 * A078360  Numbers having a unique representation as sum of a positive square
 *          and a positive cube.
 * https://oeis.org/A078360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 6871, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th integer that admits a unique representation
   as s² + c³ with s,c ≥ 1.  The routine builds all sums up to a fixed limit,
   keeps those occurring exactly once, and answers queries in O(1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the unique‑sums array */
static seq_t *unique_sums = NULL;   /* array of unique sums */
static size_t unique_len  = 0;      /* number of elements in the array   */
static size_t unique_cap  = 0;      /* allocated capacity of unique_sums */
static bool    initialized = false;/* true when the array is ready       */

/* Append a value to the dynamic array unique_sums, growing it if necessary. */
static void append_value(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        if (*cap == 0) *cap = 16;
        else           *cap <<= 1;
        *arr = (seq_t *)realloc(*arr, (*cap) * sizeof(seq_t));
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Build the list of numbers that have a unique representation. */
static void compute_terms(void)
{
    /* Upper bound sufficient to contain the first 10 000 terms. */
    const idx_t LIMIT = 10000000ULL; /* 10 million */

    /* Counter array: 0,1,2 (capped at 2) for each possible sum. */
    uint8_t *counter = (uint8_t *)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (counter == NULL) exit(EXIT_FAILURE);

    /* Enumerate all square–cube sums up to LIMIT. */
    for (idx_t s = 1;; ++s) {
        idx_t s2 = s * s;
        if (s2 > LIMIT) break;
        for (idx_t c = 1;; ++c) {
            idx_t c3 = c * c * c;
            idx_t sum = s2 + c3;
            if (sum > LIMIT) break;
            if (counter[(size_t)sum] < 2) counter[(size_t)sum]++;  /* cap at 2 */
        }
    }

    /* Collect sums with exactly one representation. */
    for (idx_t v = 1; v <= LIMIT; ++v) {
        if (counter[(size_t)v] == 1)
            append_value(&unique_sums, &unique_len, &unique_cap, (seq_t)v);
    }

    free(counter);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_terms();
    if (n == 0 || n > unique_len) return (seq_t)0;   /* undefined for out‑of‑range */
    return unique_sums[n - 1];
}
