/*
 * A141329  Infinite array read by antidiagonals: a(m,n) = the smallest
 *          positive integer that is >= max(m,n) and is non-coprime to both m
 *          and n.
 * https://oeis.org/A141329
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 540, u16 n <= 9061, u32 n <= 9061, u64 n <= 9061,
 *            u128 n <= 9061, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = the smallest integer k ≥ max(m,n) that is not coprime
   with both m and n, where (m,n) are the antidiagonal coordinates of n
   in the array starting at row 2 and column 2. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Find distinct prime factors of n (n ≥ 2).  The array must be large enough. */
static void
prime_factors(uint64_t n, uint64_t *arr, size_t *cnt)
{
    *cnt = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            arr[(*cnt)++] = d;
            while (n % d == 0) n /= d;
        }
        ++d;
    }
    if (n > 1)
        arr[(*cnt)++] = n;
}

/* Compute a(m,n) for m,n ≥ 2 following the definition. */
static seq_t
a_m_n(idx_t m_idx, idx_t n_idx)
{
    uint64_t m = (uint64_t)m_idx;
    uint64_t n = (uint64_t)n_idx;
    uint64_t lim = m > n ? m : n;          /* max(m,n) */

    uint64_t pm[64], pn[64];
    size_t cm, cn;
    prime_factors(m, pm, &cm);
    prime_factors(n, pn, &cn);

    uint64_t best = 0;
    bool first = true;

    for (size_t i = 0; i < cm; ++i) {
        uint64_t p = pm[i];
        for (size_t j = 0; j < cn; ++j) {
            uint64_t q = pn[j];
            uint64_t l = (p == q) ? p : p * q;       /* lcm(p,q) */
            uint64_t t = (lim + l - 1) / l;          /* ceil(lim/l) */
            uint64_t cand = t * l;
            if (first || cand < best) {
                best = cand;
                first = false;
            }
        }
    }
    return (seq_t)best;
}

/* External function required by the harness. */
seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;                            /* undefined for n < 2 */

    /* Map n (offset by 2) to antidiagonal coordinates (m,n). */
    idx_t k = n - 2;                         /* zero‑based index after offset */
    idx_t s = 4;                             /* current sum m + n (≥4) */
    idx_t cum = 0;                           /* number of elements before this sum */

    while (cum + (s - 3) <= k) {
        cum += s - 3;
        ++s;
    }

    idx_t offset = k - cum;                  /* offset within this sum */
    idx_t m = 2 + offset;                    /* first coordinate (≥2) */
    idx_t nn = s - m;                        /* second coordinate (≥2) */

    return a_m_n(m, nn);
}
