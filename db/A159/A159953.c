/*
 * A159953  Values in A054973 larger than 1.
 * https://oeis.org/A159953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A159953:  Values in A054973 larger than 1.
 *
 *  The function a(n) returns the n-th element of the list of
 *  multiplicities of the sigma function that are greater than one.
 *  It computes the sigma values for all integers up to a fixed limit,
 *  counts the frequency of each sigma value, and stores those frequencies
 *  that exceed one.  The first time a(idx_t) is called the list is built
 *  once and cached for subsequent calls (in any order).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cache of multiplicities that are > 1 */
static seq_t *a_values = NULL;     /* array of multiplicities   */
static size_t a_len = 0;           /* number of stored values   */
static bool initialized = false;   /* tells whether cache built */

/* --------------------------------------------------------------------- */
/*  Build the list of multiplicities > 1 for sigma values.               */
/* --------------------------------------------------------------------- */
static void build_a_values(void)
{
    if (initialized) return;

    /* -----------------------------------------------------------------
     *  Choose a limit big enough to generate at least 10 000 terms.
     *  2 000 000 is sufficient for the required range and keeps the
     *  running time under 2 s on typical machines.
     * ----------------------------------------------------------------- */
    const size_t N = 2000000;

    /* ---------- 1.  Compute sigma(n) for 1 ≤ n ≤ N ------------------- */
    uint64_t *sigma = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    if (!sigma) abort();   /* allocation failure */

    sigma[1] = 1;            /* sigma(1) = 1 */
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = i; j <= N; j += i) {
            sigma[j] += (uint64_t)i;
        }
    }

    /* ---------- 2.  Find maximum sigma value ------------------------ */
    uint64_t max_sigma = 0;
    for (size_t i = 1; i <= N; ++i) {
        if (sigma[i] > max_sigma) max_sigma = sigma[i];
    }

    /* ---------- 3.  Count frequency of each sigma value ------------- */
    size_t *freq = (size_t *)calloc(max_sigma + 1, sizeof(size_t));
    if (!freq) abort();

    for (size_t i = 1; i <= N; ++i) {
        uint64_t s = sigma[i];
        freq[s]++;                     /* freq is a size_t array */
    }

    /* ---------- 4.  Collect all frequencies > 1 into a_values ----- */
    size_t cap = 1024;
    a_values = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!a_values) abort();

    for (uint64_t k = 1; k <= max_sigma; ++k) {
        if (freq[k] > 1) {
            if (a_len == cap) {
                cap <<= 1;  /* double capacity */
                seq_t *tmp = (seq_t *)realloc(a_values, cap * sizeof(seq_t));
                if (!tmp) abort();
                a_values = tmp;
            }
            a_values[a_len++] = (seq_t)freq[k];
        }
    }

    /* ---------- 5.  Clean up temporary arrays --------------------- */
    free(sigma);
    free(freq);

    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Public interface:  a(idx_t n)                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for zero index */

    build_a_values();              /* Ensure cache is ready */

    /* Guard against out-of-range request */
    if ((size_t)n > a_len) return (seq_t)0;

    return a_values[(size_t)n - 1];
}
