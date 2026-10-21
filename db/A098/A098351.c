/*
 * A098351  Multiplication table of the composites read by antidiagonals.
 * https://oeis.org/A098351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098351 – Multiplication table of the composites read by antidiagonals
    a(n) = A002808(i) * A002808(j)
    where the pair (i,j) lies on the d‑th antidiagonal with
        d(d+1)/2 ≥ n,
    i = k,
    j = d+1-k,
    k = n – (d-1)d/2.  */

/*  Compile-time constants ------------------------------------------------- */
#define COMPOSITE_CAP  8192   /* enough for n up to 10⁷ (d ≈ 4.5k) */
#define INITIAL_CANDIDATE 4   /* first composite */

/*  Required system headers ----------------------------------------------- */
#include <stdbool.h>
#include <stddef.h>

/*  Utilities --------------------------------------------------------------- */
#include "../utils/is_prime.h"  /* is_prime(idx_t) */

static seq_t  composites[COMPOSITE_CAP];
static size_t composites_len = 0;   /* number of composites stored */
static idx_t  next_candidate   = INITIAL_CANDIDATE;

/*  Return the pos‑th composite (1‑based).  -------------------------------- */
static seq_t composite_at(idx_t pos)
{
    /*  Extend the list until we have enough composites. */
    while (composites_len < pos) {
        /*  Skip primes.  */
        while (is_prime(next_candidate))
            ++next_candidate;

        /*  Store the composite as a seq_t.  */
        composites[composites_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return composites[pos - 1];
}

/*  OEIS sequence generator ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset is 1 – return 0 for n=0 */

    /*  Find the smallest d with d(d+1)/2 ≥ n.  */
    idx_t d = 1;
    while ( ((unsigned __int128)d * (d + 1)) / 2 < n )
        ++d;

    /*  Number of terms before diagonal d.  */
    unsigned __int128 prev = ((unsigned __int128)(d - 1) * d) / 2;

    /*  Position within diagonal (1‑based).  */
    idx_t k = n - (idx_t)prev;

    /*  Antidiagonal coordinates.  */
    idx_t i = k;
    idx_t j = d + 1 - k;

    /*  Retrieve the required composites and multiply.  */
    seq_t ci = composite_at(i);
    seq_t cj = composite_at(j);

    return ci * cj;
}
