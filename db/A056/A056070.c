/*
 * A056070  Number of 5-element ordered antichains on an unlabeled n-element
 *          set; T_1-hypergraphs with 5 labeled nodes and n hyperedges.
 * https://oeis.org/A056070
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{32} c_k * C(n + k - 1, n) where c_k are
 *            coefficients computed via inclusion–exclusion on the 32 possible
 *            hyperedge types (subsets of the 5 nodes). The computation is
 *            done once and reused for every call
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"

static int pair_id[5][5];
static long long coeff[33];
static int coeff_count = 0;

static void init_coeff(void)
{
    const int d = 20;        /* number of directed pairs (i≠j) */
    const int total_types = 32; /* 2^5 subsets of nodes */

    /* Build mapping from directed pair (i,j) to bit position 0..19 */
    int idx = 0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            pair_id[i][j] = (i == j) ? -1 : idx++;

    /* count_per_mask[mask] = number of hyperedge types whose
       coverage mask equals 'mask' (mask has 20 bits) */
    static uint16_t count_per_mask[1 << 20] = {0};
    static uint16_t g[1 << 20] = {0};

    /* Compute coverage mask for each hyperedge type (subset of nodes) */
    for (int s = 0; s < total_types; ++s)
    {
        int mask = 0;
        for (int i = 0; i < 5; ++i)
            if (s & (1 << i))
                for (int j = 0; j < 5; ++j)
                    if (i != j && !(s & (1 << j)))
                        mask |= (1 << pair_id[i][j]);
        count_per_mask[mask]++;
    }

    /* SOS DP: g[B] = number of types whose mask ⊆ B */
    for (int m = 0; m < (1 << 20); ++m)
        g[m] = count_per_mask[m];

    for (int b = 0; b < 20; ++b)
    {
        int bit = 1 << b;
        for (int m = 0; m < (1 << 20); ++m)
            if (m & bit)
                g[m] += g[m ^ bit];
    }

    /* Build coefficients c_k */
    for (int i = 0; i < 33; ++i)
        coeff[i] = 0;

    for (int B = 0; B < (1 << 20); ++B)
    {
        int k = g[B];                     /* number of types avoiding B's complement */
        int sign = ((d - __builtin_popcount(B)) & 1) ? -1 : 1;
        coeff[k] += sign;
    }

    coeff_count = 1;   /* non-zero coefficients will be used later */
    return;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4 (offset 4). For other n,
       we simply return 0. */
    if (n < 4)
        return 0;

    /* Initialize coefficients on first call */
    static int initialized = 0;
    if (!initialized)
    {
        init_coeff();
        initialized = 1;
    }

    seq_t result = 0;

    for (int k = 0; k <= 32; ++k)
    {
        long long ck = coeff[k];
        if (ck == 0) continue;

        /* Compute binomial term: C(n + k - 1, n) */
        seq_t bin = binom(n + k - 1, n);

        if (ck > 0)
        {
            seq_t term = bin * (seq_t)ck;
            result += term;
        }
        else /* ck < 0 */
        {
            seq_t term = bin * (seq_t)(-ck);
            result -= term;  /* safe because final result is non-negative */
        }
    }

    return result;
}
