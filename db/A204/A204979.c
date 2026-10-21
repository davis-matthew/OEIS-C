/*
 * A204979  Least k such that n divides 2^(k-1)-2^(j-1) for some j satisfying
 *          1<=j<k.
 * https://oeis.org/A204979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 2 such that for some j, 1 ≤ j < k, * n divides
 *            2^(k-1) – 2^(j-1). (Search over j.)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) for n ≥ 1.  Uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Work with a 64‑bit copy of the index. */
    uint64_t un = (uint64_t)n;

    /* Count the power of two dividing n.  v2n = number of trailing zeros. */
    uint64_t v2n = 0;
    uint64_t tmp = un;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++v2n;
    }

    seq_t best = 0;          /* holds minimal k found so far */
    uint64_t maxj = v2n + 1;  /* j runs up to v2n+1 */

    for (uint64_t j = 1; j <= maxj; ++j) {
        /* g = 2^min(v2n, j-1) */
        uint64_t shift = (j - 1 <= v2n) ? (j - 1) : v2n;
        uint64_t g = 1ULL << shift;

        /* n_g = n / g.  It is guaranteed to be an integer. */
        uint64_t n_g = un / g;

        /* If n_g is even, no solution with this j (2^(k-j)-1 is odd). */
        if ((n_g & 1ULL) == 0ULL)
            continue;

        /* Compute the multiplicative order of 2 modulo n_g.  */
        uint64_t e;
        if (n_g == 1) {
            e = 1;                /* 2^1 ≡ 1 (mod 1) by convention */
        } else {
            uint64_t val = 2 % n_g;
            e = 1;
            while (val != 1) {
                val = (val * 2) % n_g;
                ++e;
            }
        }

        seq_t k = (seq_t)(j + e);  /* candidate k */
        if (best == 0 || k < best)
            best = k;
    }

    /* For n ≥ 1 the loop always finds a candidate. */
    return best;
}
