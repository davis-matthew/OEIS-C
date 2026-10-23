/*
 * A136027  Smallest prime of the form (p-2n)/(2n+1) with p prime.
 * https://oeis.org/A136027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1798, u32 n <= 6439, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: for each n ≥ 1, find the smallest prime q such that
   p = q*(2n+1) + 2n is also prime.  The returned value is that q. */
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t)   */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t)        */

seq_t a(idx_t n) {
    if (n == 0)                    /* undefined for n=0 – return 0 safely   */
        return 0;

    /* Compute 2n and 2n+1 once, as seq_t to avoid overflow in large builds. */
    seq_t two_n  = ((seq_t)n) * 2;  /* 2n  */
    seq_t two_np1 = two_n + 1;      /* 2n+1 */

    /* Search for the minimal prime q.  Start from the smallest prime 2. */
    uint64_t q = 2;
    for (;;) {
        if (!is_prime_small(q)) {     /* skip non‑primes quickly */
            ++q;
            continue;
        }

        /* Compute p = q*(2n+1) + 2n.  All operations are in seq_t. */
        seq_t p = ((seq_t)q) * two_np1 + two_n;

        if (is_prime_big(p)) {        /* p is prime – return q */
            return (seq_t)q;
        }
        ++q;
    }
}
