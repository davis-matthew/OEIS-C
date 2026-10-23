/*
 * A131232  a(n) is the number of positive integers < n that do not have 2 or
 *          more distinct prime divisors in common with n.
 * https://oeis.org/A131232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, first_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131232: a(n) = number of positive integers < n that do not have
 * 2 or more distinct prime divisors in common with n.
 * Implementation uses the inclusion‑exclusion identity:
 *   bad(n) = Σ_{t=2}^{k} (-1)^t (t-1) Σ_{|S|=t} ⌊(n-1)/∏_{p∈S} p⌋
 *   a(n) = (n-1) - bad(n)
 *
 * The function is O(2^k) where k is the number of distinct prime factors of n,
 * which is tiny for all practical n. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/first_bit.h"

/* Count set bits in a 64‑bit integer */
static inline int popcount64(uint64_t x) {
    return __builtin_popcountll(x);
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;           /* a(1) = 0, a(0) is undefined */

    idx_t nm1 = n - (idx_t)1;               /* numbers < n */

    /* Factor n to obtain its distinct prime factors */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt <= 1) return (seq_t)nm1;        /* no pair of common primes possible */

    /* Inclusion‑exclusion over all non‑empty subsets of the distinct primes */
    signed __int128 bad = 0;                /* intermediate signed accumulator */

    uint64_t mask_limit = ((uint64_t)1 << cnt);
    for (uint64_t mask = 0; mask < mask_limit; ++mask) {
        int t = popcount64(mask);
        if (t < 2) continue;                /* only subsets of size ≥ 2 */

        unsigned __int128 prod = 1;
        bool overflow = false;
        uint64_t m = mask;
        while (m) {
            int bit = first_bit(m);
            m &= m - 1;                     /* clear lowest set bit */
            prod *= primes[bit];
            if (prod > nm1) {               /* product already too large */
                overflow = true;
                break;
            }
        }
        if (overflow) continue;             /* contributes 0 */

        idx_t div = nm1 / (idx_t)prod;      /* floor((n‑1)/prod) */

        signed __int128 coeff = (t % 2 == 0) ? 1 : -1;  /* (-1)^t */
        coeff *= (t - 1);                   /* (t-1) factor */
        bad += coeff * (signed __int128)div;
    }

    seq_t bad_seq = (seq_t)bad;            /* bad is guaranteed non‑negative */
    return (seq_t)nm1 - bad_seq;           /* a(n) = (n-1) - bad(n) */
}
