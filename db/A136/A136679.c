/*
 * A136679  a(n) is the number of ordered solutions (x,y,z) to x^2 + y^2 ==
 *          z^2 mod n with 1 <= x,y,z <= n-1.
 * https://oeis.org/A136679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y,z) ∈ [1,n-1]^3 : x^2 + y^2 ≡ z^2 (mod n)}|
 * Range:     u8 n <= 15, u16 n <= 199, u32 n <= 215, u64 n <= 215,
 *            u128 n <= 215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts solutions by precomputing the square residues
 * modulo n, the frequency of each residue as a square of a non‑zero value,
 * and then summing over all ordered pairs (x,y).  The algorithm runs in
 * O(n²) time with O(n) additional space.  It works for all n ≥ 1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle n < 2 where the range [1,n-1] is empty. */
    if (n <= 1)
        return (seq_t)0;

    static idx_t *sq = NULL;              /* square residues modulo n */
    static seq_t *freq = NULL;            /* frequency of each residue as a square root */
    static idx_t alloc_n = 0;             /* currently allocated size */

    /* Reallocate auxiliary arrays if n exceeds the current capacity. */
    if (n > alloc_n) {
        free(sq);
        free(freq);
        sq = (idx_t *)malloc(n * sizeof(idx_t));
        freq = (seq_t *)malloc(n * sizeof(seq_t));
        if (!sq || !freq) {
            /* Memory allocation failed; return 0 as a safe fallback. */
            return (seq_t)0;
        }
        alloc_n = n;
    }

    /* Precompute square residues for 1 .. n-1. */
    for (idx_t i = 1; i < n; ++i) {
        idx_t r = (i * i) % n;
        sq[i] = r;
    }

    /* Clear frequency array. */
    for (idx_t i = 0; i < n; ++i)
        freq[i] = (seq_t)0;

    /* Count how many non‑zero z produce each residue. */
    for (idx_t z = 1; z < n; ++z)
        ++freq[sq[z]];

    /* Count ordered triples. */
    seq_t ans = (seq_t)0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t sx = sq[x];
        for (idx_t y = 1; y < n; ++y) {
            idx_t sum = sx + sq[y];
            if (sum >= n)
                sum -= n;
            ans += freq[sum];
        }
    }

    return ans;
}
