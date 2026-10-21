/*
 * A093457  Product of primes in the range [T(n-1) + 1, T(n - 1) + n], where
 *          T(n) is the n-th triangular number.
 * https://oeis.org/A093457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   product of primes p in the range [T(n-1)+1 , T(n)], where T(n) =
 *            n*(n+1)/2 is the n‑th triangular number. For n=1 the range is
 *            empty and the product is 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 35, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093457 */
  
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto() */
 
/* static cache for primes up to the largest limit requested so far */
static uint64_t *prime_list = NULL;
static size_t      prime_count = 0;
static idx_t       prime_upto  = 0;
 
/* ensure that prime_list contains all primes <= limit */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_upto)
        return;                     /* already satisfied */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
    }
    /* build_primes_upto is guaranteed to allocate a new array */
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_upto = limit;
}
 
seq_t a(idx_t n)
{
    /*  n >= 1 by definition */
    /* compute T(n-1) = (n-1)*n / 2  using seq_t to avoid overflow */
    seq_t t_prev = ((seq_t)(n - 1)) * (seq_t)n / 2;
    seq_t L = t_prev + 1;        /* lower bound of interval (inclusive) */
    seq_t U = t_prev + (seq_t)n;  /* upper bound, equals T(n) */
 
    /* ensure prime table covers at least U */
    ensure_primes_up_to((idx_t)U);        /* U fits into idx_t for n <= ~10000 */
 
    /* iterate primes and multiply those in [L, U] */
    seq_t prod = 1;
    uint64_t L64 = (uint64_t)L;
    uint64_t U64 = (uint64_t)U;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > U64)
            break;       /* primes are sorted */
        if (p >= L64)
            prod *= (seq_t)p;
    }
    return prod;
}
