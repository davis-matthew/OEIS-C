/*
 * A075512  Seventh column of triangle A075497.
 * https://oeis.org/A075512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (2^n) * S2(n+7, 7)
 *
 * Uses the recurrence for Stirling numbers of the second kind:
 *   S2(k+1, m) = m * S2(k, m) + S2(k, m-1)
 *   with S2(0,0) = 1 and S2(0,m>0) = 0.
 *
 * An array S holds the values S2(k,7) for k = 0..kmax.  The array
 * is extended on demand; each call recomputes the needed portion.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Static storage for Stirling 2‑numbers S2(k,7).                       */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> S;
#else
static seq_t *S = NULL;
static size_t S_len = 0;          /* number of entries currently valid */
#endif

/* ------------------------------------------------------------------ */
/* Compute S2(k,7) for all k = 0 .. upto and store in S.               */
/* ------------------------------------------------------------------ */
static void compute_S_from_scratch(idx_t upto)
{
    /* Allocate or reallocate storage */
#ifdef __cplusplus
    S.resize(upto + 1);
#else
    S = (seq_t*)realloc(S,
                        (size_t)(upto + 1) * sizeof(seq_t));
    if (!S) abort();                 /* allocation failure */
#endif

    /* Current Stirling values for m = 0 .. 7.  Start with k = 0. */
    seq_t s[8];
    for (idx_t i = 0; i < 8; ++i) s[i] = 0;
    s[0] = 1;                         /* S2(0,0) = 1 */

    /* Fill S[k] = S2(k,7) for k = 0 .. upto */
    for (idx_t k = 0; k <= upto; ++k)
    {
        /* Store the value for column 7 */
#ifdef __cplusplus
        S[k] = s[7];
#else
        S[k] = s[7];
#endif

        /* Compute the next row of Stirling numbers (k+1). */
        seq_t new_s[8];
        new_s[0] = 0;
        for (idx_t m = 1; m <= 7; ++m)
            new_s[m] = s[m] * (seq_t)m + s[m - 1];

        /* Move to next row */
        for (idx_t m = 0; m <= 7; ++m) s[m] = new_s[m];
    }

#ifndef __cplusplus
    S_len = (size_t)(upto + 1);
#endif
}

/* ------------------------------------------------------------------ */
/* Ensure that S contains entries up to the requested index.           */
/* ------------------------------------------------------------------ */
static void ensure_S(idx_t up_to)
{
#ifdef __cplusplus
    if (S.size() <= (size_t)up_to)
        compute_S_from_scratch(up_to);
#else
    if (S_len <= (size_t)up_to)
        compute_S_from_scratch(up_to);
#endif
}

/* ------------------------------------------------------------------ */
/* Fast power of two: 2^exp.  Acceptable for both integer and big     */
/* integer types because the multiplication by 2 works generically.   */
/* ------------------------------------------------------------------ */
static seq_t pow2_n(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 2;
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Need S2(n+7,7); compute up to index n+7. */
    idx_t need = n + 7;
    ensure_S(need);

    seq_t pow2 = pow2_n(n);
    return pow2 * S[need];
}
