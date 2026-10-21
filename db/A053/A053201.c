/*
 * A053201  Pascal's triangle (excluding first, last element of each row) read
 *          by rows, row n read mod n.
 * https://oeis.org/A053201
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 4951, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   binom_mod, u64gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the binomial coefficient C(r, k) mod r,
 * where the sequence index n (n ≥ 2) uniquely determines
 * the row r (r ≥ 2) and position k (1 ≤ k ≤ r-1) by
 *   rem   = n - 1
 *   r     = smallest integer such that r(r-1)/2 ≥ rem
 *   k     = rem - (r-1)(r-2)/2
 * i.e. the terms are the entries of Pascal's triangle 
 * (excluding the 1's at the ends) read row by row, with each
 * entry reduced modulo its row number.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom_mod.h"
#include "../utils/u64gcd.h"

/*====================================================================*/
/*  Sequence function:  seq_t a(idx_t n)  --------------------------- */
/*====================================================================*/

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;         /* indices below offset are irrelevant */

    /* ------ determine r and k from the sequence index n ------ */
    idx_t rem = n - 1;                  /* number of terms preceding the current one */

    /* find smallest r such that r*(r-1)/2 ≥ rem */
    idx_t r = 2;
    unsigned __int128 cum = 1;          /* cumulative terms up to row r: r*(r-1)/2 */
    while (cum < rem) {
        ++r;
        cum += r - 1;                   /* next C(r) = previous + (r-1) */
    }

    idx_t prev_cum = cum - (r - 1);      /* C(r-1) */
    idx_t k = rem - prev_cum;            /* position in the row (1‑based) */

    /* ------ compute the requested term ------ */
    return binom_mod(r, k, r);
}
