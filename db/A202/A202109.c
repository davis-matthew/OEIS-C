/*
 * A202109  a(n) = n^3*(n+1)^3*(n+2)^3/72.
 * https://oeis.org/A202109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n*(n+1)*(n+2)/6)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 221,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* sequence offset is 1: a(1) is the first term */
    if (n == 0) return 0;

    /* Compute n*(n+1)*(n+2)/6 in 64‑bit.  For n ≤ 10000 it is
       well below 2^64 so no overflow occurs. */
    uint64_t t = (uint64_t)n * (n + 1) * (n + 2) / 6;

#if defined(__SIZEOF_INT128__)
    /* 128‑bit builds: perform the cube and the final
       multiplication in an unsigned __int128 type to keep
       intermediate values exact. */
    unsigned __int128 cube = (unsigned __int128)t;
    cube = cube * cube * cube * 3;  /* 3 * t³ */
    return (seq_t)cube;
#else
    /* Builds without native 128‑bit support.
       seq_t is either an arbitrary‑precision type or a 128‑bit
       integer type; the operations below are performed using
       that type, which guarantees correctness. */
    seq_t tt = (seq_t)t;
    seq_t cube = tt * tt * tt;  /* t³ */
    return cube * (seq_t)3;     /* 3 * t³ */
#endif
}
