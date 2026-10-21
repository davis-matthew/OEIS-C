/*
 * A239942  a(n) = prime(n)! - prime(n - 1)!.
 * https://oeis.org/A239942
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)! - prime(n-1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0; /* value for indices below the offset is irrelevant */
    }
    /* Retrieve the two consecutive primes */
    idx_t pprev = get_prime(n - 1);
    idx_t pcur  = get_prime(n);

    /* Compute (p_{n-1})! once */
    seq_t fact_pprev = factorial((idx_t)pprev);

    /* Compute (p_n)! as (p_{n-1})! multiplied by all integers between */
    seq_t fact_pcur = fact_pprev;
    for (idx_t k = pprev + 1; k <= pcur; ++k) {
        fact_pcur = fact_pcur * (seq_t)k;
    }

    /* Return the difference */
    return fact_pcur - fact_pprev;
}
