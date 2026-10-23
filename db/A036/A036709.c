/*
 * A036709  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2,
 *          a>=0, b>=0.
 * https://oeis.org/A036709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (a,b) with a≥0, b≥0 and a²+b² ≤ n(n+1)
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of the square root for an arbitrary seq_t value.
   Uses only addition, subtraction, division, comparison, and
   the built‑in operators supported by seq_t.  */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t diff = high - low;               /* never overflows */
        seq_t mid = low + diff / 2 + (diff % 2); /* ceil((low+high)/2) */
        /* Use division to avoid overflow in mid*mid */
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t R = nn * (nn + 1);            /* R = n(n+1) */

    seq_t total = 0;
    seq_t a_max = isqrt_seq(R);

    for (seq_t a = 0; a <= a_max; ++a) {
        seq_t rem = R - a * a;          /* remaining squared distance for b */
        seq_t b_max = isqrt_seq(rem);
        total += b_max + 1;             /* include b = 0 */
    }
    return total;
}
