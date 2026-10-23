/*
 * A141809  Irregular table: Row n (of A001221(n) terms, for n>=2) consists of
 *          the largest powers that divides n of each distinct prime that
 *          divides n. Terms are arranged by the size of the distinct primes.
 *          Row 1 = (1).
 * https://oeis.org/A141809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 485, u16 n <= 7316, u32 n <= 7316, u64 n <= 7316,
 *            u128 n <= 7316, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A141809: irregular table row n (n≥2) consists of the largest powers of
   each distinct prime factor of n.  This function finds the row that
   contains the n‑th entry by accumulating the number of distinct
   prime factors for each integer, then returns the appropriate prime power.
   It uses the helper routine factor_u64 from ../utils/factor_u64.h. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)                 /* row 1 has the single term 1 */
        return (seq_t)1;

    /* ----------------------------------------------------------- */
    /* Find the row that contains the n‑th term */
    /* ----------------------------------------------------------- */
    idx_t cum = 1;              /* cumulative count of terms up to current row */
    idx_t m   = 1;              /* current row index */
    size_t cnt = 0;             /* number of distinct primes in current row */

    /* Pre‑allocate storage for factorisation */
    uint64_t primes[64];
    uint64_t exps[64];

    while (cum < n) {
        ++m;
        factor_u64((uint64_t)m, primes, exps, &cnt);
        cum += (idx_t)cnt;
    }

    /* After the loop:
       m   – row containing the n‑th term
       cnt – number of distinct primes in that row
       cum – total terms up to row m
    */
    idx_t prev_cum = cum - (idx_t)cnt;          /* terms before row m */
    idx_t offset   = n - prev_cum - 1;          /* zero‑based index within row */

    /* ----------------------------------------------------------- */
    /* Compute the offset‑th prime‑power term of row m */
    /* ----------------------------------------------------------- */
    uint64_t exp  = exps[(size_t)offset];
    seq_t    power = 1;

    for (uint64_t e = 0; e < exp; ++e)
        power *= (seq_t)primes[(size_t)offset];

    return power;
}
