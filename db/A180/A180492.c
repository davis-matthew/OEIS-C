/*
 * A180492  Product of remainders of prime(n) mod k, for k =
 *          2,3,4,...,prime(n)-1.
 * https://oeis.org/A180492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A180492
 *  Formula:  a(n) = Π_{k=2}^{p_n-1} (p_n mod k),
 *  where p_n is the n‑th prime.
 *
 *  The function below obtains the n‑th prime via get_prime(n)
 *  (see ../utils/get_prime.h) and multiplies all remainders.
 *  It is straightforward, works for all supported seq_t types,
 *  and needs no auxiliary storage or memoisation.
 */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.
       For n == 0 we return 0 (never called by the test harness). */
    if (n == 0) return 0;

    /* Get the n‑th prime.  get_prime returns a uint32_t. */
    uint32_t p = get_prime(n);

    seq_t prod = 1;                  /* start with multiplicative identity */

    /* Multiply (p % k) for every integer k = 2 … p‑1. */
    for (uint32_t k = 2; k < p; ++k) {
        prod *= (seq_t)(p % k);
    }

    return prod;
}
