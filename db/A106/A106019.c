/*
 * A106019  Primes p such that 26*p +- 3 are primes.
 * https://oeis.org/A106019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 24, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A106019
   a(n) = smallest prime p such that 26p-3 and 26p+3 are both prime.
   The sequence is defined for n >= 1 with offset 1. */
#include <stdbool.h>
#include "../utils/isprime_big.h"

/* Test whether a prime p satisfies 26p-3 and 26p+3 are prime.  */
static inline bool good_prime(seq_t p) {
    const seq_t twenty_six = (seq_t)26;
    const seq_t three      = (seq_t)3;
    seq_t v1 = twenty_six * p - three;
    seq_t v2 = twenty_six * p + three;
    return isprime_big(v1) && isprime_big(v2);
}

/* Return the n-th term of the sequence (with 1‑based index). */
seq_t a(idx_t n) {
    if (n == 0)                 /* offset is 1 – only defined for n>=1 */
        return (seq_t)0;

    seq_t count = 0;
    /* Check 2 separately, then loop over odd numbers. */
    if (isprime_big((seq_t)2) && good_prime((seq_t)2)) {
        ++count;
        if (count == (seq_t)n)
            return (seq_t)2;
    }

    for (uint64_t p = 3; ; p += 2) {
        if (!isprime_big((seq_t)p))
            continue;
        if (good_prime((seq_t)p)) {
            ++count;
            if (count == (seq_t)n)
                return (seq_t)p;
        }
    }
}
