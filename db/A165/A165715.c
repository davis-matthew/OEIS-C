/*
 * A165715  Let the prime factorization of m be m = product p(m,k)^b(m,k),
 *          where p(m,j) < p(m, j+1) for all j, the p's are the distinct
 *          primes dividing m, and each b is a positive integer. Then a(n) =
 *          product {p(n,k)^b(A165713(n), k)}.
 * https://oeis.org/A165715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k} p(n,k)^{b(A165713(n),k)}
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 60, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   count_distinct_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A165713(n) is the least integer > n having the same number
   of distinct prime factors as n.

   The function computes this value for any n ≥ 2.  It relies on the
   utility functions:
     - factor_u64          : factorises a uint64_t
     - count_distinct_primes : counts the distinct prime factors
   The implementation uses only integer arithmetic on seq_t.
*/

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/count_distinct_primes.h"

/* Helper: fast exponentiation for seq_t */
static seq_t seq_pow_fast(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Off‑set: sequence defined for n ≥ 2 – for smaller n return 0 */
    if (n < (idx_t)2)
        return (seq_t)0;

    /* 1. factorise n */
    uint64_t primes_n[64];
    uint64_t exps_n[64];
    size_t cnt_n = 0;
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);

    /* 2. find m = A165713(n) – smallest integer > n with same omega */
    idx_t m = n + 1;
    size_t omega_n = cnt_n;
    while (count_distinct_primes((uint64_t)m) != omega_n)
        ++m;

    /* 3. factorise m */
    uint64_t primes_m[64];
    uint64_t exps_m[64];
    size_t cnt_m = 0;
    factor_u64((uint64_t)m, primes_m, exps_m, &cnt_m);

    /* cnt_m must equal cnt_n; otherwise the data is inconsistent */
    /* We proceed assuming they match; if not, use the smaller count. */
    if (cnt_m != cnt_n)
        cnt_n = cnt_m < cnt_n ? cnt_m : cnt_n;

    /* 4. compute the product ∏ p(n,k)^{exps_m[k]} */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt_n; ++i) {
        seq_t p = (seq_t)primes_n[i];
        seq_t power = seq_pow_fast(p, (idx_t)exps_m[i]);
        result *= power;
    }

    return result;
}
