/*
 * A214736  Least m>0 such that n-m divides n+1+m.
 * https://oeis.org/A214736
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - max{ d | d divides (2n+1) and d <= n-1 }
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (least m>0 such that n-m divides n+1+m) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined; return 0 as a safe dummy. */
    if (n < 2)
        return 0;

    /* x = 2n + 1 */
    seq_t x = (seq_t)n * 2 + 1;

    /* Compute floor(sqrt(x)) by binary search (fits in idx_t). */
    idx_t low = 0;
    idx_t high = n;            /* sqrt(2n+1) < n for n > 2 */
    idx_t sqrt_x = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t sq = (seq_t)mid * mid;
        if (sq == x) {
            sqrt_x = mid;
            break;
        }
        if (sq < x) {
            sqrt_x = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Find the largest divisor d <= n-1 of x. */
    seq_t best_d = 1;           /* 1 always divides x and satisfies d <= n-1 */
    for (idx_t d = 1; d <= sqrt_x; ++d) {
        if (x % d == 0) {
            seq_t d1 = (seq_t)d;
            seq_t d2 = x / d;
            if (d1 <= n - 1 && d1 > best_d)
                best_d = d1;
            if (d2 <= n - 1 && d2 > best_d)
                best_d = d2;
        }
    }

    return n - best_d;
}
