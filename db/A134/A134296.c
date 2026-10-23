/*
 * A134296  Quotients A134295(p)/p = (1/p) * Sum_{k=1..p} ((p-k)!*(k-1)! -
 *          (-1)^k), where p = prime(n).
 * https://oeis.org/A134296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A134295(p) / p, where p = get_prime(n) and * A134295(p) =
 *            Σ_{k=1..p} ( (p-k)! * (k-1)! – (–1)^k ). * * We use the identity
 *            * A134295(p) = Σ_{i=0}^{p-1} ( i! * (p-1-i)! – (–1)^{i+1} ) *
 *            and compute the sum incrementally. The term i!*(p-1-i)! is *
 *            term_i = i! * (p-1-i)! * with recurrence * t...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime, prod_range
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/prod_range.h"

/* Forward declarations */
static seq_t prod_range(idx_t lo, idx_t hi);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Obtain the n-th prime */
    idx_t p = get_prime(n);

    /* Compute (p-1)! using binary splitting */
    seq_t fact = prod_range((idx_t)1, p - (idx_t)1);

    /* Sum over i = 0 .. p-1 of (i! * (p-1-i)! – (–1)^{i+1}) */
    seq_t sum = 0;
    seq_t term = fact;          /* term_0 = (p-1)! */
    for (idx_t i = 0; i < p; ++i) {
        if ( (i & 1) == 0 ) {   /* i even: add 1, <- because (–1)^{i+1} = –1 */
            sum += term + (seq_t)1;
        } else {                /* i odd: subtract 1, <– because (–1)^{i+1} = +1 */
            sum += term - (seq_t)1;
        }

        /* Prepare term_{i+1} for next iteration unless i == p-1 */
        if (i + (idx_t)1 < p) {
            /* term_{i+1} = term_i * (i+1) / (p-1-i)  (exact division) */
            idx_t numer = i + (idx_t)1;
            idx_t denom = p - (idx_t)1 - i;
            term = (term * (seq_t)numer) / (seq_t)denom;
        }
    }

    /* Divide by p to obtain a(n).  The result is integer by definition. */
    return sum / (seq_t)p;
}
