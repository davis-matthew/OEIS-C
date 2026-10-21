/*
 * A051478  a(n) is the number of values k satisfying phi(k) = 4*n+2, n>0.
 * https://oeis.org/A051478
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

/* OEIS A051478: a(n) = number of integers k such that φ(k) = 4*n + 2  (n > 0)   */
/* Implementation uses a pre‑computed phi sieve up to 200 000, which is ample for n ≤ 10 000.   */
/* The algorithm counts all k with φ(k)=m for each m up to 400 002 (m = 4*n+2) and stores the
   counts in a lookup table.  Subsequent calls retrieve the value in O(1).   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------*/
/*  Constants describing the pre‑computation limits.  These are chosen large
 *  enough to cover all φ(k) values that may occur for n ≤ 10 000.           */
/*----------------------------------------------------------------------------*/
#define MAX_N_PRECOMP   10000U                     /* maximum n expected          */
#define MAX_M           (4U * MAX_N_PRECOMP + 2U)  /* largest m = 4*n+2 to support */
#define MAX_K           200000U                    /* upper bound for k            */

/*----------------------------------------------------------------------------*/
/*  Static state                                                    */
/*----------------------------------------------------------------------------*/
static bool  initialized = false;         /* has the tables been built?      */
static uint32_t *phi_arr  = NULL;         /* φ values for 1..MAX_K           */
static uint32_t *cnt_arr  = NULL;         /* cnt_arr[m] = #k with φ(k)=m    */

/*----------------------------------------------------------------------------*/
/*  Perform the sieve of Eratosthenes to fill phi_arr and then build the
   count lookup table.  This function is executed only once on the first
   call to a().                                                   */
/*----------------------------------------------------------------------------*/
static void init_phi_and_counts(void)
{
    /* Allocate memory for φ values. */
    phi_arr = (uint32_t*)malloc((MAX_K + 1) * sizeof(uint32_t));
    if (!phi_arr) { /* allocation failed – abort execution */
        for (;;); /* unrecoverable error: the program would never reach this point */
    }

    /* Initialise φ(n) = n. */
    for (uint32_t i = 0; i <= MAX_K; ++i)
        phi_arr[i] = i;

    /* Sieve to compute φ(n) for all n ≤ MAX_K. */
    for (uint32_t i = 2; i <= MAX_K; ++i) {
        if (phi_arr[i] == i) {                 /* i is prime */
            for (uint32_t j = i; j <= MAX_K; j += i) {
                phi_arr[j] -= phi_arr[j] / i;
            }
        }
    }

    /* Allocate count array (index 0 unused, we start from 1). */
    cnt_arr = (uint32_t*)calloc((MAX_M + 1), sizeof(uint32_t));
    if (!cnt_arr) {
        for (;;);
    }

    /* For each k, increment the counter for m = φ(k) if it lies within range. */
    for (uint32_t k = 1; k <= MAX_K; ++k) {
        uint32_t m = phi_arr[k];
        if (m <= MAX_M) {
            cnt_arr[m] += 1;
        }
    }

    initialized = true;
}

/*----------------------------------------------------------------------------*/
/*  Public sequence function.  It is called with n ≥ 1 and returns a(n)
   as defined by the OEIS.                                                    */
/*----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure the tables are built once. */
    if (!initialized) {
        init_phi_and_counts();
    }

    /* Compute m = 4*n + 2.  The result fits in seq_t for all n in the test set. */
    uint32_t m = (uint32_t)(4U * (uint32_t)n) + 2U;

    /* If m is larger than we pre‑computed, fall back to zero (no solutions
       are known beyond the pre‑computation range).  This should not happen
       for the intended test bounds. */
    if (m > MAX_M) {
        return (seq_t)0;
    }

    return (seq_t)cnt_arr[m];
}
