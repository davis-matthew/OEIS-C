/*
 * A171128  A117852*A130595 as lower triangular matrices.
 * https://oeis.org/A171128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) where r and k are the row and column indices of
 *            * the flattened lower‑triangular matrix * T(n,k) = C(n,k) *
 *            A002426(n-k). * r = floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2.
 *            * A002426(m) is the m‑th central Delannoy number: *
 *            A002426(0)=1, A002426(1)=1, * A002426(n) = ((2n-1)*A0024...
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 192, u64 n <= 670,
 *            u128 n <= 2358, big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdint.h>          /* idx_t and seq_t are defined by the build */
#include "../utils/binom.h"

#ifdef __cplusplus
#include <type_traits>
#include "../utils/seq_mul.h"
#endif

/* --------------------------------------------------------------- */
/*   Storage of the sequence A002426 (central Delannoy numbers)    */
/* --------------------------------------------------------------- */

static seq_t *A002426_cache = NULL;   /* allocated array */
static idx_t  A002426_size  = 0;      /* number of values currently in the array  */
static size_t A002426_cap   = 0;      /* allocated capacity                           */

/* Ensure that A002426[0 .. max] are computed.  Uses the recurrence:   */
/*     a_n = ((2n-1)*a_{n-1} + 3*(n-1)*a_{n-2}) / n .                 */
static void ensure_A002426(idx_t max)
{
    if (max < A002426_size)
        return;

    /* Grow capacity if necessary */
    size_t new_cap = A002426_cap;
    if (new_cap == 0)
        new_cap = 2;
    while (new_cap <= (size_t)(max + 1))
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(A002426_cache, new_cap * sizeof(seq_t));
    A002426_cache = tmp;
    A002426_cap   = new_cap;

    /* Initialise the first two terms if the array is empty */
    if (A002426_size == 0) {
        A002426_cache[0] = (seq_t)1;
        A002426_size     = 1;
    }
    if (max >= 1 && A002426_size <= 1) {
        A002426_cache[1] = (seq_t)1;
        A002426_size     = 2;
    }

    /* Compute the missing terms from the current size up to max */
    for (idx_t n = A002426_size; n <= max; ++n) {
        seq_t val;
#ifdef __cplusplus
        /* C++ mode – use arbitrary‑precision multiplication (seq_mul) */
        seq_t term1 = seq_mul((seq_t)(2 * n - 1), A002426_cache[n - 1]);
        seq_t term2 = seq_mul((seq_t)(3 * (n - 1)), A002426_cache[n - 2]);
        seq_t num   = term1 + term2;
        val         = num / (seq_t)n;          /* exact division */
#else
        /* C mode – use 128‑bit intermediate to avoid overflow */
        unsigned __int128 num = (unsigned __int128)(2 * n - 1) * A002426_cache[n - 1];
        num += (unsigned __int128)3 * (n - 1) * A002426_cache[n - 2];
        val = (seq_t)(num / (unsigned __int128)n);  /* exact division */
#endif
        A002426_cache[n] = val;
    }

    A002426_size = max + 1;
}

/* --------------------------------------------------------------- */
/*   Find the row r for a given index n in the flattened matrix   */
/* --------------------------------------------------------------- */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it exceeds the target row */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* --------------------------------------------------------------- */
/*   Public entry point                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine the row r and the column k (0‑based) of the index n */
    idx_t r = find_row(n);
    idx_t k = n - (r * (r + 1)) / 2;   /* 0 ≤ k ≤ r */

    /* Compute A002426(r-k) */
    idx_t base = r - k;
    ensure_A002426(base);
    seq_t base_val = A002426_cache[base];

    /* Compute binomial coefficient C(r,k) */
    seq_t binom_val = binom(r, k);

    /* Return the product */
    return binom_val * base_val;
}
