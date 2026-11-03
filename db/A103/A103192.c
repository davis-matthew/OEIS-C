/*
 * A103192  Trajectory of 1 under repeated application of the function n ->
 *          A102370(n).
 * https://oeis.org/A103192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n - 1 + 8*floor(n/4) (n ≥ 1)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 939 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The trajectory of 1 under f = A102370, f(m) = m + Sum 2^k over the k >= 1
   with m + k == 0 (mod 2^k) (the formula by N. J. A. Sloane there). Terms
   grow about 4 per step, so the trajectory is kept in 64 bits and cached. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t f(uint64_t m)
{
    uint64_t s = m;
    for (unsigned k = 1; k < 63 && (UINT64_C(1) << k) <= m + k; ++k)
        if (((m + k) & ((UINT64_C(1) << k) - 1)) == 0) s += UINT64_C(1) << k;
    return s;
}

static uint64_t *traj = NULL;
static size_t traj_len = 0, traj_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (traj_len == 0) {
        traj_cap = 1024;
        traj = (uint64_t *)malloc(traj_cap * sizeof *traj);
        if (!traj) abort();
        traj[traj_len++] = 1;
    }
    while (traj_len < (size_t)n) {
        if (traj_len == traj_cap) {
            uint64_t *t = (uint64_t *)realloc(traj, (traj_cap *= 2) * sizeof *traj);
            if (!t) abort();
            traj = t;
        }
        traj[traj_len] = f(traj[traj_len - 1]);
        ++traj_len;
    }
    return (seq_t)traj[n - 1];
}
