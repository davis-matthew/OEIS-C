/*
 * A071630  Number of terms in the set InvPhi(8*prime(n)), where prime(n) is
 *          the n-th prime.
 * https://oeis.org/A071630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts, for a given n, the number of solutions
   x with φ(x) = 8·prime(n).  It precomputes Euler's totient values up
   to a fixed limit L, stores in an array the number of occurrences of
   each totient value, and then answers each query in O(1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides get_prime(idx_t) */

/* Adjust the limit if higher n are requested.  It must be > 8·prime(n)
   and large enough to cover all solutions of φ(x)=8·prime(n).  For
   n ≤ 10000 the bound 5 000 000 is safe. */
static const size_t LIM = 5000000;

static uint64_t *phi_arr      = NULL;        /* φ(i) for 1 ≤ i ≤ LIM */
static seq_t   *occurrences  = NULL;        /* count for each totient value */
static bool     initialized  = false;

/* One‑time initialization: compute φ(i) by sieve and count
   occurrences. */
static void init(void)
{
    if (initialized) return;

    phi_arr = (uint64_t *)malloc((LIM + 1) * sizeof(uint64_t));
    if (!phi_arr) return;          /* allocation failed */

    /* φ(i) sieve */
    for (size_t i = 0; i <= LIM; ++i) phi_arr[i] = (uint64_t)i;
    phi_arr[0] = 0;
    for (size_t i = 2; i <= LIM; ++i) {
        if (phi_arr[i] == (uint64_t)i) {        /* i is prime */
            for (size_t j = i; j <= LIM; j += i) {
                phi_arr[j] -= phi_arr[j] / i;
            }
        }
    }

    /* Allocate and count occurrences.  Index is uint64_t but fits
       into size_t because φ(i) ≤ i ≤ LIM. */
    occurrences = (seq_t *)calloc((LIM + 1), sizeof(seq_t));
    if (!occurrences) {
        free(phi_arr);
        return;
    }
    for (size_t i = 1; i <= LIM; ++i) {
        uint64_t v = phi_arr[i];
        if (v <= LIM) {
            occurrences[(size_t)v]++;   /* cast v to size_t for index */
        }
    }

    /* Clean up phi_arr as it is no longer needed. */
    free(phi_arr);
    phi_arr = NULL;

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();
    if (!initialized) return (seq_t)0;   /* fallback on OOM */

    /* Compute 8·prime(n).  The product fits in 64 bits for the
       required range. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t m = 8ull * p;
    if (m > LIM) return (seq_t)0;        /* safe guard */

    return occurrences[(size_t)m];
}
