/*
 * A059772  Smallest prime p such that n is a solution mod p of x^2 = 2, or 0
 *          if no such prime exists.
 * https://oeis.org/A059772
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p > n that divides n² – 2, 0 if none
 * Range:     u8 n <= 18, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* offset is 2, behaviour for < 2 is irrelevant */

    uint64_t nn = (uint64_t)n;           /* cast index to 64 bit for arithmetic */

    /* Compute n² – 2 safely; n is small in the test range so 128‑bit is plenty. */
    unsigned __int128 prod = (unsigned __int128)nn * (unsigned __int128)nn;
    uint64_t m = (uint64_t)(prod - 2U);  /* m = n² – 2  */

    if (m <= nn)                          /* all factors ≤ n → no qualifying prime */
        return (seq_t)0;

    /* Prime table up to the current maximum index (needed for trial division). */
    static uint64_t *prime_arr = NULL;
    static size_t   prime_cnt = 0;
    static uint64_t prime_limit = 0;

    if (nn > prime_limit) {               /* need to rebuild the table */
        free(prime_arr);                  /* free previous allocation (free(NULL) OK) */
        build_primes_upto(nn, &prime_arr, &prime_cnt);
        prime_limit = nn;
    }

    /* Trial‑divide m by primes ≤ √m; any remaining cofactor > n is the answer. */
    uint64_t rem = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            while (rem % p == 0) rem /= p;
        }
    }

    if (rem > 1 && rem > nn)              /* leftover prime factor > n */
        return (seq_t)rem;
    return (seq_t)0;
}
