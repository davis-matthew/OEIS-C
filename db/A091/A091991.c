/*
 * A091991  Minimal number of 1's that must be inserted into the binary
 *          representation of n to get a prime.
 * https://oeis.org/A091991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of 1's that must be inserted into the
 *            binary * representation of n (right of the most significant 1)
 *            so that the result * is a prime. The function searches insertion
 *            counts in increasing order * using a recursive stars‑and‑bars
 *            composition enumeration and tests each * constr...
 * Range:     u8 n <= 9796, u16 n <= 9796, u32 n <= 9796, u64 n <= 9796,
 *            u128 n <= 9796, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/isPrime.h"   /* assumes isPrime(idx_t) is available */

/* Recursive search over compositions of 'rem' into 'k' parts.
 * If a prime is found, *found is set to true and the function returns true. */
static bool search_comps(int pos, int rem, int k, int *cnt, const uint8_t *bits, bool *found)
{
    if (pos == k - 1) {
        cnt[pos] = rem;

        /* Build the candidate integer from the composition. */
        idx_t val = 0;
        /* first (most significant) bit */
        val = val * 2 + bits[0];

        for (int i = 0; i < k - 1; ++i) {
            for (int j = 0; j < cnt[i]; ++j)
                val = val * 2 + 1;
            val = val * 2 + bits[i + 1];
        }
        for (int j = 0; j < cnt[k - 1]; ++j)
            val = val * 2 + 1;

        if (isPrime(val)) {
            *found = true;
            return true;
        }
        return false;
    }

    for (int c = 0; c <= rem; ++c) {
        cnt[pos] = c;
        if (search_comps(pos + 1, rem - c, k, cnt, bits, found))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* out of offset range – arbitrary */

    /* Determine binary representation of n: most significant bit first. */
    int k = 0;
    idx_t tmp = n;
    while (tmp) {
        ++k;
        tmp >>= 1;
    }
    if (k == 0) k = 1;               /* n == 0 case, not used for OEIS offset */

    uint8_t bits[64];
    for (int i = 0; i < k; ++i)
        bits[i] = (n >> (k - 1 - i)) & 1U;

    int cnt[64];
    bool found = false;

    for (int t = 0; ; ++t) {         /* search increasing number of insertions */
        found = false;
        if (search_comps(0, t, k, cnt, bits, &found))
            return (seq_t)t;
    }
}
