/*
 * A136626  For every number n in Ulam's spiral the sequence gives the number
 *          of primes around it (number n excluded).
 * https://oeis.org/A136626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of prime indices among the 8 neighbors of * n in
 *            the Ulam spiral (n itself excluded)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime, xy_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"            /* is_prime(idx_t) */
#include "../utils/xy_to_idx.h"

/* Integer square root of a 64‑bit unsigned integer.
 * Return floor(sqrt(x)). Uses binary search and __int128 for safety. */
static idx_t isqrt_uint64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Convert spiral index n (1‑based) to cartesian coordinates (x, y). */
static void idx_to_xy(idx_t n, long long *x, long long *y)
{
    if (n == 1) { *x = 0; *y = 0; return; }

    idx_t sqrt_n_1 = isqrt_uint64(n - 1);
    idx_t r = (sqrt_n_1 + 1) >> 1;             /* ring radius */

    unsigned __int128 tmp = (unsigned __int128)(2 * r - 1) * (2 * r - 1);
    idx_t low = (idx_t)(tmp + 1);              /* first number in ring */

    idx_t k = n - low;                         /* offset within ring */

    long long rl = (long long)r;
    if (k < 2 * r) {                           /* right side, moving up */
        *x = rl;
        *y = -rl + 1 + (long long)k;
    } else if (k < 4 * r) {                    /* top side, moving left */
        *y = rl;
        *x = rl - 1 - ((long long)k - 2 * r);
    } else if (k < 6 * r) {                    /* left side, moving down */
        *x = -rl;
        *y = rl - 1 - ((long long)k - 4 * r);
    } else {                                   /* bottom side, moving right */
        *y = -rl;
        *x = -rl + 1 + ((long long)k - 6 * r);
    }
}

/* Public API for sequence A136626. */
seq_t a(idx_t n)
{
    long long x, y;
    seq_t count = 0;

    idx_to_xy(n, &x, &y);

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            long long nx = x + dx;
            long long ny = y + dy;
            idx_t idx = xy_to_idx(nx, ny);
            if (idx >= 1 && is_prime(idx)) {
                ++count;
            }
        }
    }
    return count;
}
