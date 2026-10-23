/*
 * A194401  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) < 0, where
 *          r=(1+sqrt(5))/2 and < > denotes fractional part.
 * https://oeis.org/A194401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194401
    Numbers m such that Sum_{k=1..m} ( <1/2 + k*r> - <k*r> ) < 0,
    where  r = (1+sqrt(5))/2  and  <x> is the fractional part of x.
    The sequence is given by a(1) = 1, a(2) = 3, a(3) = 9, ...
*/

/*  The condition S(m) < 0 can be rewritten as:
        low(m) < m/2 ,
    where  low(m) = #{ 1 ≤ k ≤ m | {k*r} < 1/2 }.
    With r = φ,  {k*φ} < 1/2  iff  floor( (k + floor(k*√5))/2 ) ==
                                 floor( (k + floor(k*√5) + 1)/2 ).
    The integer  floor(k*√5) is easily obtained by an exact integer square‑root.
*/

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static state to avoid recomputing for successive calls */
    static seq_t *terms = NULL;        /* array of found terms a(1),a(2),...   */
    static size_t  terms_len = 0;      /* current number of stored terms      */
    static idx_t   next_m    = 1;      /* next m to test                      */
    static uint64_t low       = 0;      /* low(count of {k*φ} < 1/2) up to (next_m-1) */

    /* enlarge the array when necessary */
    if (terms_len == 0) {
        terms = (seq_t *)malloc(sizeof(seq_t) * 16);
        if (!terms) { return (seq_t)0; }
        terms_len = 0;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* compute floor(k*√5) for k = next_m */
        /* use 5*k^2 because (k*√5)^2 = 5*k^2 */
        seq_t sq = (seq_t)5 * (seq_t)next_m * (seq_t)next_m;
        seq_t s  = floor_sqrt(sq);          /* s = ⌊√(5*k²)⌋ = ⌊k*√5⌋ */

        /* integer floor of k*φ and k*φ+½ without floating point */
        seq_t floor_kphi  = ((seq_t)next_m + s) >> 1;
        seq_t floor_kphi1 = (((seq_t)next_m + s + 1) >> 1);

        /* {k*φ} < ½  iff the two floors are equal */
        if (floor_kphi == floor_kphi1) {
            low++;                      /* one more k satisfies the inequality   */
        }

        /* condition S(k) < 0  ⇔  low < k/2   ⇔  2*low < k  */
        if ((low * 2) < next_m) {
            /* store the valid m */
            if (terms_len == 16) {
                /* double the capacity */
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * 32);
                if (!terms) { return (seq_t)0; }
            } else if (terms_len == 32) {
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * 64);
                if (!terms) { return (seq_t)0; }
            }
            terms[terms_len++] = (seq_t)next_m;
        }

        next_m++;   /* move to the next integer m */
    }

    return terms[n-1];
}
