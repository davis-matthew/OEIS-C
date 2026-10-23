/*
 * A123584  Largest digit in any base b expansion of n, 2 <= b <= sqrt(n).
 * https://oeis.org/A123584
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = largest digit in the base‑b representation of n, for all
 *            bases 2 ≤ b ≤ ⌊√n⌋. The sequence is defined for n ≥ 4
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)).  Safe for 64‑bit values. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid != 0 && mid > n / mid) {  /* mid*mid would overflow or > n */
            high = mid - 1;
        } else {
            res = mid;
            low = mid + 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The specification states that behavior for n < 4 is irrelevant. */
    if (n < 4)
        return 0;

    idx_t limit   = isqrt_idx(n);   /* ⌊√n⌋ */
    idx_t maxdigit = 0;

    for (idx_t base = 2; base <= limit; ++base) {
        idx_t tmp = n;
        while (tmp) {
            idx_t d = tmp % base;
            if (d > maxdigit)
                maxdigit = d;
            tmp /= base;
        }
    }

    return (seq_t)maxdigit;
}
