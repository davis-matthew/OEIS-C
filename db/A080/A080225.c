/*
 * A080225  Number of perfect divisors of n.
 * https://oeis.org/A080225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ d | n : sigma(d) = 2d }| (perfect divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Known perfect numbers that fit in 64‑bit integers. */
    static const uint64_t perfects[] =
    {
        6ULL,                     /* 2^(2‑1)·(2^2‑1)   */
        28ULL,                    /* 2^(3‑1)·(2^3‑1)   */
        496ULL,                   /* 2^(5‑1)·(2^5‑1)   */
        8128ULL,                  /* 2^(7‑1)·(2^7‑1)   */
        33550336ULL,              /* 2^(13‑1)·(2^13‑1) */
        8589869056ULL,            /* 2^(17‑1)·(2^17‑1) */
        137438691328ULL,          /* 2^(19‑1)·(2^19‑1) */
        2305843008139952128ULL    /* 2^(31‑1)·(2^31‑1) */
    };
    static const size_t perfect_cnt = sizeof(perfects) / sizeof(perfects[0]);

    seq_t count = 0;
    for (size_t i = 0; i < perfect_cnt; ++i)
    {
        if (n % perfects[i] == 0)
            ++count;
    }
    return count;
}
