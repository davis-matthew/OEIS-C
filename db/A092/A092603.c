/*
 * A092603  a(n) = Sum_{k=1..n} min(k!, binomial(n,k)).
 * https://oeis.org/A092603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} min(k!, binomial(n,k))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the GNU-style binomial function from the utils library */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but harmless */

    seq_t sum = 0;   /* running total */
    seq_t fact = 1;  /* k! for the current k, starts at 1! */
    bool fact_valid = true;        /* true while factorial fits and is <= binomial */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t b = binomial(n, k);              /* C(n,k) */

        /* choose the smaller of factorial and binomial */
        if (fact_valid && fact <= b) {
            sum += fact;
        } else {
            sum += b;
            fact_valid = false;                /* factorial no longer useful */
        }

        /* compute next factorial, if it may still be needed */
        if (fact_valid && k < n) {
            seq_t nxt = fact * (k + 1);

            /* detect overflow: if division does not recover the original factor, we overflowed */
            if (nxt / (k + 1) != fact) {
                fact_valid = false;
            } else {
                fact = nxt;
            }
        }
    }

    return sum;
}
