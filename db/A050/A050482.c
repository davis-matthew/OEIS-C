/*
 * A050482  Sum of remainders when n-th prime is divided by all preceding
 *          integers.
 * https://oeis.org/A050482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{p_n-1} (p_n mod k)
 * Range:     u8 n <= 12, u16 n <= 111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_n is the n‑th prime (offset 1: a(1) = 0). */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Return the sequence value a(n).  External linkage is required. */
seq_t a(idx_t n)
{
    /* a(0) is undefined for this sequence; return 0 to avoid UB */
    if (n == 0) {
        return 0;
    }

    /* Retrieve the n‑th prime.  get_prime returns uint32_t. */
    uint32_t p = get_prime(n);

    /* Sum the remainders p mod k for k = 1 .. p‑1. */
    seq_t sum = 0;
    for (uint32_t k = 1; k < p; ++k) {
        sum += (seq_t)(p % k);
    }

    return sum;
}
