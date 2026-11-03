/*
 * A073256  a(n) = phi(n-th composite number).
 * https://oeis.org/A073256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n-th composite number)
 * Range:     u8 n <= 225, u16 n <= 1200, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Validated: standard, pass, 89 terms through n = 1191 (2026-11-01)
 * Helpers:   is_prime, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the n-th composite integer, then return its Euler totient. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* n is 1-based index of the sequence. */
    if (n == 0) return 0;   /* undefined, but guard against underflow */

    idx_t count = 0;
    idx_t candidate = 4;     /* first composite number */

    while (true) {
        if (!is_prime(candidate)) {   /* composite */
            count++;
            if (count == n) {
                /* cast to seq_t before passing to calc_phi */
                return totient_seq((seq_t)candidate);
            }
        }
        /* Safe to increment; candidate fits in idx_t for all practical n. */
        candidate++;
    }
}
