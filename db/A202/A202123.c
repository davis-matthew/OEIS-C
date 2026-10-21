/*
 * A202123  Number of -7..7 arrays of n elements with first, second and third
 *          differences also in -7..7.
 * https://oeis.org/A202123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A202123 – Number of arrays of length n with entries in –7..7
 *  where first, second, and third differences are also in –7..7.
 *
 *  This implementation uses dynamic programming over the last three
 *  elements of the array.  States are indexed by the last three
 *  values (x,y,z) ∈ {–7,…,7}³.  The transition from state (x,y,z) to
 *  a longer state (y,z,w) checks the constraints for the new first,
 *  second, and third differences.  The algorithm runs in O(n) time
 *  with 15³ ≈ 3 375 states and requires only O(15³) memory.
 *
 *  The function is completely self‑contained and does not rely on
 *  any external helpers.  It works for any build where seq_t and
 *  idx_t are integer types, and it is correct for all n ≥ 1.
 */
#include <stdint.h>

enum { S = 15, SHIFT = 7, TOT = S * S * S };

/* Encode a triple (x,y,z) with values in –7..7 into a
   unique index 0..TOT-1. */
static inline int state_idx(int x, int y, int z)
{
    return ((x + SHIFT) * S + (y + SHIFT)) * S + (z + SHIFT);
}

/* Decode an index into the triple (x,y,z). */
static inline void state_verts(int idx, int *x, int *y, int *z)
{
    int v = idx;
    *z = (v % S) - SHIFT;
    v /= S;
    *y = (v % S) - SHIFT;
    *x = (v / S) - SHIFT;
}

seq_t a(idx_t n)
{
    /* Arrays for dynamic programming.  Their size is constant
       at compile time (TOT = 3 375) so they may be static or
       automatic.  We use automatic storage for portability. */
    seq_t dp1[S];
    seq_t dp2[S * S];
    seq_t dp3[TOT];
    seq_t ndp[TOT];

    /* Initialise dp1: sequences of length 1. */
    for (int i = 0; i < S; ++i) dp1[i] = 1;

    /* Length 1 answer. */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < S; ++i) total += dp1[i];
        return total;
    }

    /* Initialise dp2: sequences of length 2.  The only constraint
       is that the first difference lies in –7..7. */
    for (int i = 0; i < S * S; ++i) dp2[i] = 0;
    for (int a = -7; a <= 7; ++a) {
        for (int b = -7; b <= 7; ++b) {
            if (b - a >= -7 && b - a <= 7) {
                dp2[(a + SHIFT) * S + (b + SHIFT)] = 1;
            }
        }
    }

    /* Length 2 answer. */
    if (n == 2) {
        seq_t total = 0;
        for (int i = 0; i < S * S; ++i) total += dp2[i];
        return total;
    }

    /* Initialise dp3: sequences of length 3.  Constraints are on
       first and second differences. */
    for (int i = 0; i < TOT; ++i) dp3[i] = 0;
    for (int idx = 0; idx < S * S; ++idx) {
        seq_t cnt = dp2[idx];
        if (!cnt) continue;
        int a, b;
        a = (idx / S) - SHIFT;
        b = (idx % S) - SHIFT;
        for (int c = -7; c <= 7; ++c) {
            if (c - b < -7 || c - b > 7) continue;
            int d2 = c - 2 * b + a;
            if (d2 < -7 || d2 > 7) continue;
            dp3[state_idx(a, b, c)] += cnt;
        }
    }

    /* Length 3 answer. */
    if (n == 3) {
        seq_t total = 0;
        for (int i = 0; i < TOT; ++i) total += dp3[i];
        return total;
    }

    /* For n >= 4, propagate DP from length 3 to length n. */
    for (int len = 4; len <= n; ++len) {
        /* Zero the temporary array. */
        for (int i = 0; i < TOT; ++i) ndp[i] = 0;
        /* Transition from each state (x,y,z) to (y,z,w). */
        for (int idx = 0; idx < TOT; ++idx) {
            seq_t cnt = dp3[idx];
            if (!cnt) continue;
            int x, y, z;
            state_verts(idx, &x, &y, &z);
            for (int w = -7; w <= 7; ++w) {
                int d1 = w - z;
                if (d1 < -7 || d1 > 7) continue;
                int d2 = w - 2 * z + y;
                if (d2 < -7 || d2 > 7) continue;
                int d3 = w - 3 * z + 3 * y - x;
                if (d3 < -7 || d3 > 7) continue;
                ndp[state_idx(y, z, w)] += cnt;
            }
        }
        /* Swap dp3 and ndp for the next iteration. */
        for (int i = 0; i < TOT; ++i) dp3[i] = ndp[i];
    }

    /* Final answer: sum all counts in dp3. */
    seq_t total = 0;
    for (int i = 0; i < TOT; ++i) total += dp3[i];
    return total;
}
