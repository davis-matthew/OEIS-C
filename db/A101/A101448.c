/*
 * A101448  Nonnegative numbers k such that 2k + 11 is prime.
 * https://oeis.org/A101448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Sequence A101448
 *
 *   a(n) = the n-th non‑negative integer k such that 2*k + 11 is prime.
 *   Offset 1: a(1) = 0, a(2) = 1, ...
 *
 *   This implementation iterates k from 0 upward, testing each
 *   candidate prime 2*k+11 with the provided is_prime() routine.
 *   It stops when the n‑th such k has been found and returns it
 *   cast to seq_t.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1, but return 0 for n = 0 just in case */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k = 0;

    while (true) {
        uint64_t cand = 2ULL * (uint64_t)k + 11ULL;    /* 2*k + 11 */

        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)k;                     /* found the n‑th element */
        }
        ++k;
    }
}
