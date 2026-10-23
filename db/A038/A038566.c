/*
 * A038566  Numerators in canonical bijection from positive integers to
 *          positive rationals <= 1: arrange fractions by increasing
 *          denominator then by increasing numerator.
 * https://oeis.org/A038566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, core, nice, tabf
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 9627, u32 n <= 9627, u64 n <= 9627,
 *            u128 n <= 9627, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A038566: numerators of positive rationals ≤ 1
   arranged by increasing denominator and then increasing numerator.
   For each n we find the smallest denominator d such that
   Σ_{k=1..d} φ(k) ≥ n, then take the (n – Σ_{k<d} φ(k))‑th integer i≤d
   with gcd(i,d)=1.  This uses a sieve for φ and a cumulative sum array
   that grows as needed.  The implementation is valid C11 and C++17
   and works for any seq_t type provided by the build.  It does not
   require acceleration beyond the sieve. */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of totient and cumulative sums */
    static uint64_t *phi  = NULL;   /* phi[i] = φ(i) for i≥1 (fits in uint64_t) */
    static seq_t   *cum  = NULL;   /* cum[i] = Σ_{k=1..i} φ(k) */
    static idx_t    limit = 0;     /* current size of cached arrays */

    /* --------------------------------------------------------------
       Grow the cache until we have cum[limit] ≥ n.
       -------------------------------------------------------------- */
    while (cum == NULL || cum[limit] < (seq_t)n) {
        idx_t new_limit = (limit == 0) ? 2 : limit * 2;

        /* allocate new arrays */
        uint64_t *new_phi = (uint64_t *)malloc((new_limit + 1) *
                                                sizeof(uint64_t));
        seq_t    *new_cum = (seq_t    *)malloc((new_limit + 1) *
                                                sizeof(seq_t));
        if (!new_phi || !new_cum) {
            exit(1); /* allocation failure */
        }

        /* initialise phi[i] = i */
        for (idx_t i = 0; i <= new_limit; ++i) new_phi[i] = i;
        /* sieve to compute φ */
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (new_phi[p] == p) { /* p is prime */
                for (idx_t j = p; j <= new_limit; j += p)
                    new_phi[j] -= new_phi[j] / p;
            }
        }
        /* cumulative sums */
        new_cum[0] = 0;
        for (idx_t i = 1; i <= new_limit; ++i)
            new_cum[i] = new_cum[i-1] + (seq_t)new_phi[i];

        /* replace old arrays */
        free(phi);
        free(cum);
        phi   = new_phi;
        cum   = new_cum;
        limit = new_limit;
    }

    /* --------------------------------------------------------------
       Binary search for the smallest d with cum[d] ≥ n.
       -------------------------------------------------------------- */
    idx_t low  = 1;
    idx_t high = limit;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum[mid] >= (seq_t)n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t denom = low;
    seq_t prev_sum = cum[denom - 1];
    idx_t k = (idx_t)((seq_t)n - prev_sum); /* rank within this denominator */

    /* --------------------------------------------------------------
       Find the k‑th integer coprime to denom.
       -------------------------------------------------------------- */
    idx_t counter = 0;
    for (idx_t i = 1; i <= denom; ++i) {
        if (gcd((seq_t)i, (seq_t)denom) == (seq_t)1) {
            ++counter;
            if (counter == k)
                return (seq_t)i;
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
