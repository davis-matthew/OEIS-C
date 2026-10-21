/*
 * A095389  a(n) is the number of residues from reduced residue system, R,
 *          modulo 210 such that both R and R+2 are primes, i.e., both 210n+r
 *          and 210n+r+2 are primes at fixed n.
 * https://oeis.org/A095389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095389: count of reduced residues r modulo 210 for which
   210*n+r and 210*n+r+2 are both prime.  The formula is
   a(n) = |\{ r : 0 ≤ r < 210, gcd(r,210)=1, P(210*n+r)=P(210*n+r+2)=1 \}|.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Euclidean algorithm for 64‑bit values, used for the residue test. */

/* Prime sieve maintained statically and grown as needed. */
static idx_t sieve_limit = 0;
static uint8_t *isPrime = NULL;

/* Expand the sieve to cover all numbers up to 'limit' (inclusive). */
static void grow_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    uint8_t *new_arr = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!new_arr) return;               /* we silently fail on OOM */
    memset(new_arr, 1, (limit + 1) * sizeof(uint8_t));
    new_arr[0] = 0;
    if (limit >= 1) new_arr[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_arr[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                new_arr[k] = 0;
        }
    }

    free(isPrime);
    isPrime = new_arr;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    /* Determine the maximum prime that might be tested for this n. */
    idx_t max_needed = 210 * n + 211;             /* r = 209 gives p1 = 210*n+209, p2 = 210*n+211 */
    grow_sieve(max_needed);

    seq_t count = 0;

    for (int r = 0; r < 210; ++r) {
        /* Only reduced residues modulo 210. */
        if (r == 0) continue;
        if (gcd_u64((uint64_t)r, 210ULL) != 1u) continue;

        idx_t p1 = 210 * n + r;
        idx_t p2 = p1 + 2;

        if (isPrime[p1] && isPrime[p2]) {
            ++count;
        }
    }

    return count;
}
