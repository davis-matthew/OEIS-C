/*
 * A160720  Number of "ON" cells at n-th stage in 2-dimensional cellular
 *          automaton (see Comments for precise definition).
 * https://oeis.org/A160720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 495, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160720: Number of "ON" cells at n-th stage in the diagonal cellular automaton.
   The sequence satisfies:
       a(0) = 0
       a(1) = 1
       a(n) = a(n-1) + 4*( 2^{popcount(n-1)} - 1 ),   for n ≥ 2
   where popcount(k) is the number of 1‑bits in the binary representation of k.
   Equivalent closed form:
       a(n) = 1 + Σ_{k=0}^{n-1} 4*( 2^{popcount(k)} - 1 ).
   The recurrence is linear in the sense that each new term is obtained by adding a
   signed integer that can be computed in O(1) time from the index n-1.
   This implements the automaton rules exactly: a cell turns ON only if it has
   exactly one diagonal neighbour ON in the previous stage. */

#include <stdint.h>          /* 
                               seq_t and idx_t are defined by the build system
                               (e.g. uint64_t, uint8_t, unsigned __int128, …). 
                               We do not define them here.                     */

static inline unsigned popcnt_idx(idx_t x)
{
    /* idx_t is always <= 64‑bit; use built‑in popcount for safety. */
    return __builtin_popcountll((unsigned long long)x);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 0: a(0) = 0 */

    seq_t result = 1;             /* a(1) = 1 */
    if (n == 1)
        return result;

    /* Compute successive differences using the recurrence formula. */
    for (idx_t i = 2; i <= n; ++i)
    {
        unsigned pc = popcnt_idx(i - 1);      /* popcount of (i-1) */
        unsigned long long pow2 = 1ULL << pc; /* 2^{pc}, pc ≤ 64 for n ≤ UINT64_MAX */
        unsigned long long term = 4ULL * (pow2 - 1ULL); /* 4*(2^{pc}-1) */
        result += (seq_t)term;
    }

    return result;
}
