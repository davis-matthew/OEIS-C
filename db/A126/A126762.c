/*
 * A126762  a(n) is the least k > n such that the remainder when n^k is
 *          divided by k is n.
 * https://oeis.org/A126762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 250, u16 n <= 996, u32 n <= 996, u64 n <= 996,
 *            u128 n <= 996, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A126762: a(n) = least k > n such that n^k ≡ n (mod k)
 * (OEIS offset 1)
 *
 * Implementation: Brute‑force search from n+1 upward, testing the modular
 * congruence using the provided big‑modular exponentiation routine.
 * This is efficient for n up to at least 10 000 and respects all type
 * constraints.
 */
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* use seq_t for n for arithmetic below */
    idx_t k = n + 1;              /* start searching from n+1 */

    /* Special case for n == 0 is irrelevant (offset is 1) */
    while (true) {
        /* Compute n^k mod k and compare to n */
        seq_t res = mod_pow(nn, k, (seq_t)k);
        if (res == nn) {
            return (seq_t)k;
        }
        ++k;
    }
}
