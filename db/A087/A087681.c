/*
 * A087681  Numbers n such that n + 6 and n - 6 are both prime.
 * https://oeis.org/A087681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) is the nth integer such that n-6 and n+6 are both prime. */
/* The function searches sequentially for candidates until the n‑th such
   number is found.  This method works for seq_t up to 64‑bit and also
   for arbitrary‑precision seq_t as the candidate is stored in a uint64_t
   and then cast to seq_t. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* Candidates start at the smallest value that can satisfy the
       condition: 11 (since 11-6 = 5 and 11+6 = 17).  All candidates are
       odd, so we step by 2. */
    uint64_t cand = 11;
    idx_t count = 0;

    while (true) {
        if (is_prime_u64(cand - 6) && is_prime_u64(cand + 6)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;   /* cast to seq_t (unsigned) */
            }
        }
        cand += 2;   /* only odd values need be tested */
    }
}
