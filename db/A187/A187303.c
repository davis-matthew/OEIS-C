/*
 * A187303  Number of 8-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9878*n^2 - 79388*n + 143388 for n > 13 a(1)=0, a(2)=0,
 *            a(3)=72, a(4)=2028, a(5)=12778, a(6)=37124, a(7)=81362,
 *            a(8)=145100, a(9)=231552, a(10)=338044, a(11)=465734,
 *            a(12)=613212, a(13)=780752
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle non‑positive index directly (though OEIS offset is 1). */
    if (n == 0) return 0;

    /* Pre‑computed values for 1 ≤ n ≤ 13 */
    static const seq_t base[] = {
        (seq_t)0,  /* a(1) */
        (seq_t)0,  /* a(2) */
        (seq_t)72, /* a(3) */
        (seq_t)2028, /* a(4) */
        (seq_t)12778, /* a(5) */
        (seq_t)37124, /* a(6) */
        (seq_t)81362, /* a(7) */
        (seq_t)145100,/* a(8) */
        (seq_t)231552,/* a(9) */
        (seq_t)338044,/* a(10) */
        (seq_t)465734,/* a(11) */
        (seq_t)613212,/* a(12) */
        (seq_t)780752 /* a(13) */
    };

    if (n <= 13) return base[n - 1];

    /* Compute quadratic formula without overflow.
       Using __int128 guarantees enough room for n up to ~10^5
       (the values comfortably fit into 64‑bit for all tested n). */
    uint64_t nn = (uint64_t)n;
    __int128 t = (__int128)9878 * nn * nn
               - (__int128)79388 * nn
               + 143388;
    return (seq_t)t;
}
