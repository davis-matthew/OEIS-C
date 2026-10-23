/*
 * A126601  a(n) = n-th composite from among those composites which are
 *          coprime to n.
 * https://oeis.org/A126601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th composite that is coprime to n
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function scans successive integers, selecting composites
 * that are coprime to the input index n, until the n‑th one is found. */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Treat n as seq_t for gcd comparison */
    seq_t sn = (seq_t)n;

    seq_t count = 0;
    seq_t candidate = 4;          /* first composite number */

    for (;; ++candidate) {
        /* Skip numbers < 4 – already satisfied by initialization */
        if (!is_prime_small(candidate)) { /* candidate is composite */
            if (gcd(candidate, sn) == 1) {
                ++count;
                if (count == n) {
                    return candidate;
                }
            }
        }
    }
}
