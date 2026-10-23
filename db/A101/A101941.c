/*
 * A101941  Peak-trough transform of the omega sequence (A001221).
 * https://oeis.org/A101941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime–trough transform of the omega sequence (A001221).
 * For n ≥ 1:  a(n) =   k   if ω(n) > both neighbors and > the k‑nearest
 *                     neighbors on each side;
 *                 = -k   if ω(n) < both neighbors and < the k‑nearest
 *                     neighbors on each side;
 *                 =  0   otherwise.
 *  ω(n) = number of distinct prime factors of n (ω(1)=0).
 *
 * The routine keeps a static sieve of ω up to the largest index
 * requested.  The sieve is recomputed from scratch whenever a larger
 * limit is needed.
 *
 * This implementation satisfies the practical time limits for n up to
 * about 10 000, with O(n log log n) preprocessing per new maximum n.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for the omega sieve */
static seq_t *omega = NULL;   /* Ω[0] unused, Ω[n] stored as seq_t */
static idx_t omega_len = 0;   /* highest stored index (≥0) */

/* Ensure Ω[0..limit] is computed; recompute from scratch if limit grows. */
static void ensure_omega(idx_t limit)
{
    if (limit <= omega_len)          /* already sufficient */
        return;

    /* Allocate/resize */
    seq_t *tmp = (seq_t *)realloc(omega, (limit + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failed */
    omega = tmp;

    /* Initialise all entries to 0 */
    for (idx_t i = 0; i <= limit; ++i)
        omega[i] = 0;

    /* Sieve for distinct prime factors (ω(n)).  For each prime p add
       1 to all multiples of p. */
    for (idx_t p = 2; p <= limit; ++p) {
        if (omega[p] == 0) {         /* p is prime */
            for (idx_t j = p; j <= limit; j += p)
                omega[j] += (seq_t)1;
        }
    }

    omega_len = limit;
}

/* Peak‑trough transform of the ω sequence */
seq_t a(idx_t n)
{
    if (n < 3)                /* not enough neighbours to be a peak/trough */
        return 0;

    /* We may need ω up to n + (n-1) = 2n-1 for the outward scan. */
    ensure_omega((idx_t)(2 * n - 1));

    seq_t om   = omega[n];
    seq_t omL  = omega[n - 1];
    seq_t omR  = omega[n + 1];

    bool isPeak   = (om > omL) && (om > omR);
    bool isTrough = (om < omL) && (om < omR);

    if (!isPeak && !isTrough)
        return 0;            /* neither peak nor trough */

    idx_t k = 0;
    for (idx_t d = 1; d <= n - 1; ++d) {
        seq_t left  = omega[n - d];
        seq_t right = omega[n + d];

        if (isPeak) {
            if (om > left && om > right)
                ++k;
            else
                break;
        } else { /* trough */
            if (om < left && om < right)
                ++k;
            else
                break;
        }
    }

    return isPeak ? (seq_t)k : (seq_t)(-((seq_t)k));
}
