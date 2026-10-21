/*
 * A216841  Smallest palindromic number of length 2 in two bases differing by
 *          n.
 * https://oeis.org/A216841
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 6359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm for A216841: smallest number m that is a 2‑digit palindrome
   in two bases differing by n. */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for idx_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a   = b;
        b   = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;                /* offset is 2 */

    seq_t best = 0;                     /* 0 indicates unset */
    bool found = false;

    /* Enumerate all divisors d of n (except 1). */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* Process this divisor d = i. */
        if (i > 1) {
            idx_t d = i;
            idx_t k = n / d;              /* k = n / d   (integer) */
            /* t_start = ceil((k+2)/(d-1)) = (k + d) / (d-1) */
            idx_t t = (k + d) / (d - 1);
            if (t == 0) t = 1;           /* safety, though t ≥ 1 always */

            /* Find the smallest t ≥ t_start coprime to k. */
            while (gcd_idx(t, k) != 1) ++t;

            seq_t cand = (seq_t)d * (seq_t)t * (seq_t)(t + k);
            if (!found || cand < best) {
                best = cand;
                found = true;
            }
        }

        idx_t d2 = n / i;                 /* paired divisor */
        if (d2 == i || d2 <= 1) continue; /* skip duplicates and 1 */

        idx_t d = d2;
        idx_t k = n / d;
        /* t_start = (k + d) / (d-1) */
        idx_t t = (k + d) / (d - 1);
        if (t == 0) t = 1;

        while (gcd_idx(t, k) != 1) ++t;

        seq_t cand = (seq_t)d * (seq_t)t * (seq_t)(t + k);
        if (!found || cand < best) {
            best = cand;
            found = true;
        }
    }

    return best;                          /* guaranteed to be set for n≥2 */
}
