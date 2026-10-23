/*
 * A175447  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=13.
 * https://oeis.org/A175447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime > a(n-1) such that a(n)+a(n-1) ≡ 0 (mod
 *            13); a(1)=2
 * Range:     u8 n <= 5, u16 n <= 658, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t len    = 0;          /* last computed index (1‑based) */

    /* Return cached value if already computed */
    if (n <= len)
        return cache[n];

    /* Make sure the array can hold indices up to n */
    ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));

    /* Initialise the sequence if it hasn't been done yet */
    if (len == 0) {
        cache[1] = (seq_t)2;
        len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = len + 1; k <= n; ++k) {
        seq_t prev = cache[k - 1];

        /* Desired residue for the next prime: p + prev ≡ 0 (mod 13) */
        int prev_mod    = (int)(prev % 13);
        int target_mod  = (13 - prev_mod) % 13;

        /* Start from the next integer after prev, adjusted to the target residue */
        seq_t cand = prev + 1;
        int cand_mod = (int)(cand % 13);
        int delta = (target_mod - cand_mod + 13) % 13;
        cand = cand + delta;

        /* Find the first prime in this residue class */
        while (!isPrimeGeneric(cand))
            cand = cand + 13;

        cache[k] = cand;
    }

    len = n;
    return cache[n];
}
