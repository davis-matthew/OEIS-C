/*
 * A073909  Smallest number m such that m and the product of digits of m are
 *          both divisible by 2n, or 0 if no such number exists.
 * https://oeis.org/A073909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A085124(2*n) where A085124(m) = smallest m such that m
 *            and the product of its digits are both divisible by m, or 0 if
 *            no such m exists
 * Range:     u8 n <= 6, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation assumes that 0 is NOT divisible by any number
   (as stated in the OEIS entry).  It checks the special cases
   described in the comments:  a(n)=0 if n is divisible by 5 or
   contains a prime divisor >9.

   The function iterates over multiples of 2*n and tests each candidate.
   A candidate is acceptable only if the product of its digits is
   non‑zero and divisible by 2*n.

   The implementation uses the library routine `digit_product`
   provided by the build system.  No other external helpers are required. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"

/* Return the n-th term of OEIS A073909 */
seq_t a(idx_t n)
{
    /* Off‑by‑one safety; the sequence is defined for n >= 1 */
    if (n == 0)
        return 0;

    /* If n is a multiple of 5 the term is 0, per the comments. */
    if (n % 5ULL == 0)
        return 0;

    /* Reject n that contains a prime factor > 9.
       Only the primes 2, 3, 5, 7 are allowed. */
    uint64_t rem = n;
    /* Remove all factors of 2, 3, 7 (prime 5 already handled). */
    while (rem % 2ULL == 0)
        rem /= 2ULL;
    while (rem % 3ULL == 0)
        rem /= 3ULL;
    while (rem % 7ULL == 0)
        rem /= 7ULL;
    /* If anything remains, it is either a prime > 9 or a composite
       whose prime factors are > 9, in both cases we return 0. */
    if (rem > 1)
        return 0;

    /* Target divisor: 2*n */
    seq_t target = (seq_t)2 * (seq_t)n;

    /* Search successively for a multiple of target that also
       satisfies the product‑of‑digits divisibility condition. */
    for (seq_t m = target; ; m += target) {
        seq_t prod = digit_product(m);
        if (prod && (prod % target) == 0)
            return m;
    }
}
