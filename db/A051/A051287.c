/*
 * A051287  Triangular array T read by rows: T(n,k)=P(n,k,|n-2k|), where
 *          P(n,k,c)=number of vectors (x(1),x(2,),...,x(n)) of k 1's and n-k
 *          0's such that x(i)=x(n+1-i) for exactly c values of i.
 * https://oeis.org/A051287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051287: Triangular array T(n,k)=P(n,k,|n-2k|), read by rows.
 * For a given index n, we find the row r and column k, and compute
 * T(r,k) combinatorially.
 *
 * Let m = floor(r/2), center = r%2 (1 if r odd else 0).
 * Let c  = |r-2k|.
 * Then the number of equal pairs is e = (c - center)/2.
 * If c < center or e > m, the entry is 0.
 *
 * For each possible center bit value (if r is odd; only 0 if r even)
 * we solve
 *       k = 2*s + (m - e) + center_bit          (1)
 * where s = #equal pairs set to 1.
 * If (k-(m-e)-center_bit) is even and non‑negative and
 *    s <= e, the contribution is
 *        C(m,e) * C(e,s) * 2^(m-e).
 * The total is the sum over the allowed center_bit values.
 *
 * All arithmetic is performed with idx_t for indices and seq_t
 * for counts (the seq_t type is large enough for all values). */ 
#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that (r*(r+1))/2 <= n < ((r+1)*(r+2))/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;           /* column index */

    idx_t m = r / 2;
    idx_t center = r % 2;          /* 0 if r even, 1 if r odd */

    /* Absolute difference |r-2k| */
    idx_t doublek = 2 * k;
    idx_t c = (r >= doublek) ? (r - doublek) : (doublek - r);

    if (c < center) {
        return 0;
    }
    idx_t e = (c - center) / 2;    /* number of equal pairs */

    if (e > m) {
        return 0;
    }

    seq_t total = 0;

    /* For even r, center_bit must be 0 (no middle element). */
    if (center == 0) {
        /* center_bit = 0 */
        idx_t s_num = k - (m - e);   /* numerator for 2*s */
        if (s_num >= 0 && (s_num & 1) == 0) {
            idx_t s = s_num >> 1;
            if (s <= e) {
                seq_t val = binom(m, e);
                val *= binom(e, s);
                val <<= (m - e);
                return val;
            }
        }
        return 0;
    }

    /* For odd r, we must consider center_bit = 0 and 1 */
    for (idx_t cv = 0; cv <= 1; ++cv) {
        idx_t s_num = k - (m - e) - cv;
        if (s_num >= 0 && (s_num & 1) == 0) {
            idx_t s = s_num >> 1;
            if (s <= e) {
                seq_t val = binom(m, e);
                val *= binom(e, s);
                val <<= (m - e);
                total += val;
            }
        }
    }
    return total;
}
