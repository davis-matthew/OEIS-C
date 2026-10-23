/*
 * A187763  Number of common terms in consecutive Collatz trajectories.
 * https://oeis.org/A187763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |trajectory(n) ∩ trajectory(n+1)|, the number of common
 *            terms in the Collatz trajectories of n and n+1. The Collatz step
 *            is defined as x → x/2 if x is even, x → 3·x+1 if x is odd. This
 *            implementation works for any seq_t type (fixed‑size or arbitrary
 *            precision) and for idx_t = uint64_t
 * Range:     u8 n <= 25, u16 n <= 5658, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort – compares two seq_t values.  No conflict with
   externally defined names. */
static int cmp_seq_t_local(const void *p1, const void *p2)
{
    const seq_t v1 = *(const seq_t *)p1;
    const seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

/* Compute the number of common elements in the Collatz trajectories
   of n and n+1. */
seq_t a(idx_t n)
{
    /* Upper bound on length of trajectories for n ≤ 10000.
       4096 is sufficient for all known cases. */
    const idx_t MAX_TRAJ = 4096;

    seq_t traj_n[MAX_TRAJ];
    seq_t traj_np1[MAX_TRAJ];
    idx_t len_n = 0, len_np1 = 0;

    /* Helpers for constants that may participate in arithmetic. */
    const seq_t ONE = 1;
    const seq_t THREE = 3;

    /* Build trajectory for n. */
    seq_t x = (seq_t)n;
    while (1) {
        if (len_n >= MAX_TRAJ) break;          /* safety guard */
        traj_n[len_n++] = x;
        if (x == ONE) break;
        if ((x & 1) == 0)   x = x >> 1;
        else                x = THREE * x + ONE;
    }

    /* Build trajectory for n+1. */
    x = (seq_t)(n + (idx_t)1);
    while (1) {
        if (len_np1 >= MAX_TRAJ) break;         /* safety guard */
        traj_np1[len_np1++] = x;
        if (x == ONE) break;
        if ((x & 1) == 0)   x = x >> 1;
        else                x = THREE * x + ONE;
    }

    /* Sort both trajectories to allow linear‑time intersection. */
    qsort(traj_n,  (size_t)len_n,  sizeof(seq_t), cmp_seq_t_local);
    qsort(traj_np1, (size_t)len_np1, sizeof(seq_t), cmp_seq_t_local);

    /* Count common elements. */
    idx_t i = 0, j = 0;
    idx_t count = 0;
    while (i < len_n && j < len_np1) {
        if (traj_n[i] < traj_np1[j])   i++;
        else if (traj_n[i] > traj_np1[j]) j++;
        else { /* equal */ count++; i++; j++; }
    }

    return (seq_t)count;
}
