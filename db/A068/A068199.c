/*
 * A068199  One of a family of sequences that interpolates between the Bell
 *          numbers and the factorials.
 * https://oeis.org/A068199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 2 * ∑_{k=1}^{n-1} S(n-1,k) * 3^k,
 *            where S(m,k) are Stirling numbers of the second kind
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   reallocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include needed headers */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/reallocate_seq_array.h"

/* Helpers for memory allocation in C and C++ */
#if defined(__cplusplus)
#define ALLOC_SEQ_ARRAY(n) (new seq_t[(n)])
#define FREE_SEQ_ARRAY(p)  (delete [] (p))
#else
#define ALLOC_SEQ_ARRAY(n) ( (seq_t*)malloc((n) * sizeof(seq_t)) )
#define FREE_SEQ_ARRAY(p)  ( free(p) )
#endif

/* Global static state: Stirling numbers (row n) and powers of 3 */
static seq_t* S = NULL;          /* S[k] = S(n,k) for current n */
static size_t S_cap = 0;         /* allocated capacity of S */
static idx_t  current_n = 0;     /* the n for which S holds the Stirling row */

static seq_t* pow3 = NULL;       /* pow3[i] = 3^i */
static size_t pow3_cap = 0;      /* allocated capacity of pow3 */
static idx_t  current_pow = 0;   /* the largest i where pow3 is defined */

/* Ensure that Stirling numbers are computed up to n (inclusive) */
static void ensure_stirling(idx_t n)
{
    if (n <= current_n) return;

    for (idx_t m = current_n + 1; m <= n; ++m) {
        /* Resize arrays if necessary */
        size_t needed = (size_t)m + 1;
        if (needed > S_cap) {
            seq_t* newS = reallocate_seq_array(S, S_cap, needed);
            S = newS;
            S_cap = needed;
        }
        /* Compute new Stirling row m from previous row (m-1) in-place
           using descending update to keep old values needed. */
        for (idx_t k = m - 1; k > 0; --k) {
            S[k] = k * S[k] + S[k - 1];
        }
        S[m] = 1;  /* S(m,m) = 1 */
        S[0] = 0;  /* S(m,0) = 0 for m > 0 */
        current_n = m;
    }
}

/* Ensure powers of 3 are computed up to i (inclusive) */
static void ensure_powers(idx_t i)
{
    if (i <= current_pow) return;

    for (idx_t j = current_pow + 1; j <= i; ++j) {
        size_t needed = (size_t)j + 1;
        if (needed > pow3_cap) {
            seq_t* newP = reallocate_seq_array(pow3, pow3_cap, needed);
            pow3 = newP;
            pow3_cap = needed;
        }
        pow3[j] = pow3[j - 1] * 3;
        current_pow = j;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Initialize static arrays on first call */
    if (S == NULL) {
        S = ALLOC_SEQ_ARRAY(1);
        S[0] = (seq_t)1;          /* S(0,0) */
        S_cap = 1;
        current_n = 0;
    }
    if (pow3 == NULL) {
        pow3 = ALLOC_SEQ_ARRAY(1);
        pow3[0] = (seq_t)1;        /* 3^0 */
        pow3_cap = 1;
        current_pow = 0;
    }

    idx_t want = n - 1;           /* we need Bell number B_{n-1}(3) */

    ensure_stirling(want);
    ensure_powers(want);

    seq_t B;
    if (want == 0) {
        B = (seq_t)1;             /* B_0(3) = 1 */
    } else {
        B = (seq_t)0;
        for (idx_t k = 1; k <= want; ++k) {
            B += S[k] * pow3[k];
        }
    }

    return (seq_t)2 * B;
}
