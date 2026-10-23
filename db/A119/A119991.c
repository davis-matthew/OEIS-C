/*
 * A119991  a(n) = n-th positive integer which is coprime to the n-th
 *          composite positive integer.
 * https://oeis.org/A119991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer coprime to the n-th composite
 *            integer
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime_small.h"
#include "../utils/gcd.h"

/* Find the n‑th composite integer (composites start at 4). */
static seq_t nth_composite(idx_t n)
{
    if (n == 0) return 0;          /* offset guard – not used for valid n ≥ 1 */
    idx_t cnt = 0;
    idx_t cand = 4;
    while (cnt < n)
    {
        /* cand is composite ⇔ not prime (for cand ≥ 4). */
        if (!is_prime_small((seq_t)cand))
        {
            ++cnt;
            if (cnt == n) break;
        }
        ++cand;
    }
    return (seq_t)cand;
}

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety guard – offset starts at 1 */
    seq_t m = nth_composite(n);               /* n‑th composite */

    idx_t desired = n;                         /* we need the n‑th coprime */
    idx_t found   = 0;
    for (seq_t x = 1;; ++x)
    {
        if (gcd(x, m) == 1)
        {
            ++found;
            if (found == desired) return x;
        }
    }
}
