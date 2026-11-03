/*
 * A238918  Number of self-inverse permutations p on [n] where the maximal
 *          displacement of an element equals 7.
 * https://oeis.org/A238918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of self-inverse permutations on [n] whose maximal
 *            displacement equals 7. It is computed as the difference between
 *            the number of involutions with all displacements ≤ 7 and those
 *            with all displacements ≤ 6
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 739 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic programming that keeps, for each processed position,
   a bitmask describing the pending transpositions that are still unmatched.
   The bitmasks represent distances (1…maxD) from the current position to the other
   element of the pending transposition.  The DP ensures that no pending element is ever
   allowed to exceed the maximum allowed displacement. */

/* Dependencies:
   The code relies on the types seq_t and idx_t defined in the build environment
   and on standard integer operators.  No external libraries are required. */
#include <stdbool.h>

/* count_leq_displacement returns the number of involutions on [n] whose
   maximal displacement is ≤ maxD.  maxD is 6 or 7 for this sequence. */
static seq_t count_leq_displacement(idx_t n, int maxD)
{
    const int SZ = 1 << maxD;           /* number of possible masks (2^maxD) */
    seq_t dp[SZ];
    seq_t ndp[SZ];
    for (int i = 0; i < SZ; ++i) dp[i] = 0;
    dp[0] = 1;                          /* start with no pending transpositions */

    const int FULL = SZ - 1;
    const int MAX_BIT_MASK = 1 << (maxD - 1);   /* bit corresponding to distance = maxD */

    for (idx_t pos = 1; pos <= n; ++pos)
    {
        for (int i = 0; i < SZ; ++i) ndp[i] = 0;

        for (int mask = 0; mask < SZ; ++mask)
        {
            seq_t cur = dp[mask];
            if (!cur) continue;

            bool has_max = (mask & MAX_BIT_MASK) != 0;

            /* 1) Current position is a fixed point */
            if (!has_max)
            {
                int newmask = (mask << 1) & FULL;
                ndp[newmask] += cur;
            }

            /* 2) Start a new pending transposition at the current position
               (the new pair will be matched at some future point). */
            if (!has_max)
            {
                int newmask = ((mask << 1) & FULL);
                newmask |= 1;                     /* new pending at distance 1 */
                ndp[newmask] += cur;
            }

            /* 3) Match the current position with an existing pending element.
               Each pending element of distance d (1 ≤ d ≤ maxD) may be paired. */
            int remaining = mask;
            while (remaining)
            {
                int lowbit = remaining & -remaining;      /* isolate lowest set bit */
                remaining ^= lowbit;

                int mask_without = mask & ~lowbit;         /* remove this pending */

                /* After removal, no pending element may be at the maximal distance
                   because that would become distance maxD+1 after the shift. */
                if (mask_without & MAX_BIT_MASK) continue;

                int newmask = (mask_without << 1) & FULL;
                ndp[newmask] += cur;
            }
        }

        /* Advance to the next position */
        for (int i = 0; i < SZ; ++i) dp[i] = ndp[i];
    }

    return dp[0];                      /* all pending transpositions must be matched */
}

seq_t a(idx_t n)
{
    /* For n < 8 the maximal possible displacement is < 7, so the answer is 0. */
    if (n < 8) return 0;

    seq_t f7 = count_leq_displacement(n, 7);
    seq_t f6 = count_leq_displacement(n, 6);
    return f7 - f6;
}
