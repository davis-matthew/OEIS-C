/*
 * A077105  Number of nonisomorphic generalized Petersen P(n,k) graphs on 2n
 *          nodes for 1<=k<=floor((n-1)/2).
 * https://oeis.org/A077105
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct orbits of k ∈ [1,⌊(n‑1)/2⌋] under the
 *            action k ↦ ±k, k ↦ ±k⁻¹ (mod n). In other words, a(n) counts how
 *            many non‑isomorphic generalized Petersen graphs P(n,k) exist for
 *            1 ≤ k ≤ ⌊(n‑1)/2⌋
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   find_parent, gcd, union_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/find_parent.h"
#include "../utils/union_set.h"

static idx_t modinv(idx_t a, idx_t mod)
{
    /* extended Euclidean algorithm – returns inverse modulo mod
       if it exists, otherwise 0.  */
    int64_t t   = 0;
    int64_t newt = 1;
    int64_t r   = (int64_t)mod;
    int64_t newr = (int64_t)(a % mod);

    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp;

        tmp = newt;
        newt = t - q * newt;
        t = tmp;

        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0;          /* no inverse */
    if (t < 0) t += (int64_t)mod; /* make positive */
    return (idx_t)t;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset is 3 – values below are irrelevant */

    idx_t up = (n - 1) / 2;        /* floor((n-1)/2) */
    idx_t *parent = (idx_t *)malloc((up + 1) * sizeof(idx_t));
    if (!parent) return 0;

    for (idx_t i = 1; i <= up; ++i) parent[i] = i;

    for (idx_t k = 1; k <= up; ++k) {
        idx_t candidates[4];
        int   cnt = 0;

        /* k itself */
        candidates[cnt++] = k;

        /* n - k */
        candidates[cnt++] = n - k;

        /* k inverse modulo n, if it exists */
        idx_t g = (idx_t)gcd((seq_t)k, (seq_t)n);
        if (g == 1) {
            idx_t inv = modinv(k, n);
            if (inv != 0) {
                candidates[cnt++] = inv;
                candidates[cnt++] = n - inv;
            }
        }

        /* union all canonical representatives of the candidates */
        for (int i = 0; i < cnt; ++i) {
            idx_t m = candidates[i];
            idx_t canon = (m <= up) ? m : (n - m);  /* map back into [1,up] */
            union_set(parent, k, canon);
        }
    }

    /* count distinct roots */
    seq_t result = 0;
    for (idx_t i = 1; i <= up; ++i)
        if (parent[i] == i) ++result;

    free(parent);
    return result;
}
