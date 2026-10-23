/*
 * A139902  Primes of the form 16x^2+16xy+19y^2.
 * https://oeis.org/A139902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, grow_seq, is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A139902: primes of the form 16·x² + 16·x·y + 19·y²
 *
 * The function generates primes in increasing order and keeps them in a
 * static cache.  For each candidate prime p it tests whether there
 * exist integers x,y that satisfy the quadratic form.  The test uses the
 * equivalent conditions:  (p−19·y²) must be divisible by 16, the
 * discriminant D = y² + 4·((p−19·y²)/16) must be a perfect square, and
 * (sqrt(D)−y) must be even.  The search over y is from −⌊√(p/15)⌋ to
 * +⌊√(p/15)⌋, which guarantees that all representations are found
 * because the quadratic form is positive‑definite.
 *
 * The algorithm is O(n·√p) where p is the n‑th prime of the required
 * form and is well below the 2‑second limit for n up to ~10⁴.
 *
 * Dependencies:
 *   ../utils/is_prime_generic.h
 *   ../utils/floor_sqrt.h
 *   ../utils/grow_seq.h
 *
 * Note:  All arithmetic involving the candidate prime p and the
 * intermediate quantities are performed with 128‑bit signed integers
 * (__int128) to avoid overflow for 64‑bit primes.  The discriminant
 * D always fits into a signed 64‑bit integer, so it can be safely
 * converted to seq_t for the square‑root function.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_generic.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset is 1 */

    static seq_t *primes = NULL;
    static size_t cap   = 0;
    static size_t cnt   = 0;
    static idx_t  last  = 0;       /* last integer examined */

    while (cnt < n)
    {
        last++;

        /* only residues 19 and 91 modulo 120 can be prime of the form */
        unsigned rem = (unsigned)(last % 120);
        if (rem != 19 && rem != 91) continue;

        if (!is_prime_generic(last)) continue;

        seq_t p = last;                      /* current prime candidate */

        /* bound for y: 15·y² <= p  => |y| <= floor(√(p/15)) */
        seq_t maxy_sq = floor_sqrt(p / 15);
        int64_t maxy = (int64_t)maxy_sq;

        bool has_representation = false;

        for (int64_t y = -maxy; y <= maxy; ++y)
        {
            int64_t abs_y = y >= 0 ? y : -y;
            __int128 yy   = (__int128)abs_y * abs_y;          /* y² */

            __int128 diff = (__int128)p - (__int128)19 * yy;   /* p – 19y² */

            if (diff % 16 != 0) continue;                    /* must be divisible by 16 */

            __int128 tp   = diff / 16;                        /* (p–19y²)/16, can be negative */

            __int128 D    = yy + (__int128)4 * tp;            /* discriminant */

            if (D < 0) continue;                             /* discriminant < 0 → no real x */

            seq_t D_seq   = (seq_t)D;
            seq_t sqrtD   = floor_sqrt(D_seq);

            if ((__int128)sqrtD * sqrtD != D) continue;       /* not a perfect square */

            /* parity check: (sqrtD - y) must be even */
            if ((((__int128)sqrtD - (__int128)y) & 1) != 0) continue;

            /* all conditions satisfied → p can be represented */
            has_representation = true;
            break;
        }

        if (has_representation)
        {
            grow_seq(&primes, &cap, cnt + 1);
            primes[cnt++] = p;
        }
    }

    return primes[n - 1];
}
