/*
 * A068717  a(n) = -1 if A067280(n) == 0 (mod 2), otherwise a(n) = A049240(n).
 * https://oeis.org/A068717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -1 if A067280(n) == 0 (mod 2), otherwise a(n) =
 *            A049240(n)
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:   a(n) = 0   if n is a perfect square;          */
/*                    otherwise return -1 if the period of the          */
/*                    continued‑fraction expansion of √n is odd;        */
/*                    else return 1.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root: floor(√v) for a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL;          /* 2^32 – 1, the largest possible integer sqrt of a 64‑bit value */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, but defensive */

    /* Check for perfect square. */
    idx_t root = isqrt_u64(n);
    if (root * root == n) {
        return (seq_t)0;
    }

    /* Compute the period length of the continued fraction of √n. */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a0 = root;
    idx_t a_cur = a0;
    idx_t period = 0;

    while (true) {
        /* m = d * a_cur - m */
        unsigned __int128 prod = (unsigned __int128)d * a_cur;
        m = (idx_t)(prod - m);

        /* d = (n - m*m) / d */
        unsigned __int128 msq = (unsigned __int128)m * m;
        unsigned __int128 numer = (unsigned __int128)n - msq;
        d = (idx_t)(numer / d);

        /* a_cur = (a0 + m) / d */
        a_cur = (idx_t)((a0 + m) / d);

        period++;

        /* The period ends when a_cur == 2*a0. */
        if (a_cur == 2 * a0) {
            break;
        }
    }

    /* If period is odd, negative Pell equation is solvable -> a(n) = -1 */
    if (period & 1U) {
        return (seq_t)-1;
    }
    /* Otherwise a(n) = 1 */
    return (seq_t)1;
}
