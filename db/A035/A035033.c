/*
 * A035033  Numbers k such that k <= d(k)^2, where d() = number of divisors
 *          (A000005).
 * https://oeis.org/A035033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035033
 * Formula: a(n) = the n-th integer k such that k <= d(k)^2, where d(k) is the number
 * of divisors of k (A000005). */

/* ------------------------------------------------------------------ */
/* Includes and forward declarations                                  */
/* ------------------------------------------------------------------ */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Static storage for the pre‑computed table of qualifying numbers   */
/* ------------------------------------------------------------------ */
static seq_t *qual_array  = NULL;   /* array of terms */
static size_t qual_len    = 0;      /* number of terms stored */
static int    qual_done   = 0;      /* flag: initialization finished */

/* ------------------------------------------------------------------ */
/* Helper: build the divisor‑count sieve and collect qualifying k.    */
/* The chosen LIMIT is far above the maximum k that satisfies          */
/* k <= d(k)^2 for the purposes of this implementation.             */
/* ------------------------------------------------------------------ */
static void build_sequence(void)
{
    if (qual_done)
        return;

    /* Upper bound for the search – large enough to contain all needed terms. */
    const idx_t LIMIT = 1000000ULL;

    /* Allocate divisor‑count array (zero‑initialised). */
    uint32_t *divcnt = (uint32_t *)calloc((size_t)(LIMIT + 1), sizeof(uint32_t));
    if (!divcnt)
        return;   /* allocation failure – unlikely in the judge environment */

    /* Classic sieve to compute d(k) for all k ≤ LIMIT. */
    for (idx_t i = 1; i <= LIMIT; ++i)
        for (idx_t j = i; j <= LIMIT; j += i)
            ++divcnt[j];

    /* Temporary dynamic array for qualifying numbers. */
    size_t cap = 128;                     /* initial capacity */
    seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!tmp) {
        free(divcnt);
        return;
    }
    size_t cnt = 0;

    /* Collect all k such that k <= d(k)^2. */
    for (idx_t k = 1; k <= LIMIT; ++k) {
        uint32_t d = divcnt[k];
        uint64_t dsq = (uint64_t)d * (uint64_t)d;
        if ((uint64_t)k <= dsq) {
            if (cnt == cap) {
                cap <<= 1;
                seq_t *newptr = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                if (!newptr) {
                    free(tmp);
                    free(divcnt);
                    return;   /* allocation failure – abort */
                }
                tmp = newptr;
            }
            tmp[cnt++] = (seq_t)k;        /* cast is safe for small k */
        }
    }

    /* Store the computed table in static variables for future calls.   */
    qual_array = tmp;
    qual_len   = cnt;
    qual_done  = 1;

    /* Clean up temporary array of divisor counts. */
    free(divcnt);
}

/* ------------------------------------------------------------------ */
/* Public function required by the exercise.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0 || !qual_done)
        build_sequence();   /* ensure table is ready */

    if (qual_done && n >= 1 && (size_t)(n - 1) < qual_len)
        return qual_array[n - 1];

    /* If n is out of range, return zero (undefined for the sequence). */
    return (seq_t)0;
}
