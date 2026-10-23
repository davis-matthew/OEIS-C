/*
 * A034233  Number of quaternary codes of length 2 with n words.
 * https://oeis.org/A034233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gen_permutations, init_fixed_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A034233:  Number of quaternary codes of length 2 with n words
 *
 *  The code uses Burnside’s lemma.  The group G is the product of
 *  two copies of the symmetric group on 4 symbols (acting independently
 *  on the two coordinates) together with the coordinate‑swap.
 *  G has |G| = 24·24·2 = 1152 elements.  For a group element g the
 *  permutation of the 16 words is determined by g; let L₁,…,L_c be the
 *  lengths of its cycles.  A subset of size n is fixed by g iff it is a
 *  union of whole cycles, i.e. the cycle lengths chosen sum to n.
 *  The number of such subsets is
 *
 *          A_g(n) = [yⁿ] ∏_{i=1}^c (1 + y^{L_i})
 *
 *  The desired number of orbits is
 *
 *          a(n) = (1/|G|) · Σ_{g∈G} A_g(n)
 *
 *  Since the total number of words is 16, a(n)=0 for n>16.  The
 *  algorithm pre‑computes, for every group element, the array
 *
 *      fixed[g][k] = A_g(k)   (k = 0..16)
 *
 *  and then returns the average.  The tables are small; all
 *  intermediate values fit into uint64_t (≤ 12870), so the
 *  implementation works for any seq_t type that supports the usual
 *  integer arithmetic.
 */

#include <stdbool.h>
#include "../utils/gen_permutations.h"
#include "../utils/init_fixed_counts.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n > 16) return 0; /* no code can contain more than 16 distinct words */

    static bool init = false;
    static seq_t fixed_counts[1152][17];

    if (!init) {
        init_fixed_counts(fixed_counts);
        init = true;
    }

    /* Sum over all group elements and divide by |G| = 1152 */
    seq_t sum = 0;
    int idx = (int)n;          /* safe: n <= 16 */
    for (int g = 0; g < 1152; ++g) {
        sum = sum + fixed_counts[g][idx];
    }
    return sum / (seq_t)1152;
}
