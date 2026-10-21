/*
 * A087270  Solutions to gcd(x,pi(x)) = gcd(x, A000720(x)) > 1. Numbers x such
 *          that x and pi(x) have common divisor larger than one.
 * https://oeis.org/A087270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by sieving for numbers x where gcd(x, pi(x)) > 1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/gcd.h"

/* Global state for the dynamic sieve / sequence cache */
static size_t pi_limit = 0;           /* up to which `x` we have sieved */
static bool   *prime      = NULL;    /* sieve of Eratosthenes flags   */
static uint32_t *pi_arr  = NULL;      /* cumulative prime counts       */
static seq_t   *qual      = NULL;    /* collected qualifying numbers  */
static size_t qual_size = 0;          /* number of terms currently stored */
static size_t qual_cap  = 0;          /* allocated capacity of `qual`   */

/* ------------------------------------------------------------------ */
/* Recreate all tables up to `limit`. Builds the sieve, prime‑count array
   and the list of qualifying numbers (`qual`).                           */
static void rebuild_limit(size_t limit)
{
    /* free old data if any */
    free(prime);
    free(pi_arr);
    free(qual);

    /* allocate new storage */
    prime  = (bool*)malloc((limit + 1) * sizeof(bool));
    pi_arr = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!prime || !pi_arr) return;                 /* allocation failed */

    /* initialise sieve */
    memset(prime, 1, (limit + 1) * sizeof(bool));
    prime[0] = prime[1] = 0;

    /* Eratosthenes */
    for (size_t p = 2; p * p <= limit; ++p)
        if (prime[p])
            for (size_t m = p * p; m <= limit; m += p)
                prime[m] = 0;

    /* build cumulative prime counts and qualifying list */
    qual_cap = 16;
    qual = (seq_t*)malloc(qual_cap * sizeof(seq_t));
    if (!qual) { free(prime); free(pi_arr); return; }

    qual_size = 0;
    uint32_t count = 0;
    seq_t one = (seq_t)1;

    for (size_t i = 0; i <= limit; ++i) {
        if (prime[i]) ++count;
        pi_arr[i] = count;

        if (i >= 2) {
            seq_t x = (seq_t)i;
            seq_t pi_val = (seq_t)pi_arr[i];
            if (gcd(x, pi_val) > one) {
                if (qual_size == qual_cap) {
                    qual_cap <<= 1;
                    qual = (seq_t*)realloc(qual, qual_cap * sizeof(seq_t));
                }
                qual[qual_size++] = x;
            }
        }
    }

    pi_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that the list `qual` contains at least `n` terms.  If not,
   double the sieve limit until enough terms are generated.           */
static void ensure_terms(size_t n)
{
    if (qual_size >= n) return;

    size_t limit = pi_limit > 0 ? pi_limit : 20000;
    while (qual_size < n) {
        rebuild_limit(limit);
        if (qual_size >= n) break;
        limit <<= 1;          /* grow sieve limit */
    }
}

/* ------------------------------------------------------------------ */
/* Public accessor required by the build.                              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, unused case */
    ensure_terms((size_t)n);              /* guarantees at least n terms */
    return qual[(size_t)n - 1];
}
