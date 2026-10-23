/*
 * A132239  Primes congruent to {17, 19} mod 30.
 * https://oeis.org/A132239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 17 or 19 modulo 30
   (OEIS A132239, offset 1).  The function runs in
   O(n log n) time, using the provided is_prime helper. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for offset 0 */

    idx_t  count = 0;             /* how many qualified primes seen */
    idx_t  k     = 0;             /* current 30‑block index */
    seq_t  result = 0;            /* will hold the nth prime */

    /* Loop until the nth prime is found */
    while (count < n) {
        /* candidate 30*k + 17 */
        uint64_t cand1_u64 = 30ULL * (uint64_t)k + 17ULL;
        if (is_prime((idx_t)cand1_u64)) {
            count++;
            if (count == n) {
                result = (seq_t)cand1_u64;
                break;
            }
        }

        /* candidate 30*k + 19 */
        uint64_t cand2_u64 = 30ULL * (uint64_t)k + 19ULL;
        if (is_prime((idx_t)cand2_u64)) {
            count++;
            if (count == n) {
                result = (seq_t)cand2_u64;
                break;
            }
        }

        ++k;
    }

    return result;
}
