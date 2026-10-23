/*
 * A033055  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑4 digits d(i) satisfy
 *            d(m−i) odd for all odd i (m = index of the most significant
 *            digit)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* check whether x has the required property (x > 0) */
static inline bool has_property(idx_t x)
{
    if (x == 0)                 /* 0 is not part of the sequence */
        return false;
    idx_t t = x;
    int digit[64];              /* enough for all 64‑bit numbers */
    int len = 0;
    while (t > 0) {
        digit[len++] = t & 3;   /* t % 4 */
        t >>= 2;                /* t / 4 */
    }
    int m = len - 1;            /* highest digit index */

    /* digits at indices of opposite parity to m must be odd */
    for (int j = 0; j <= m; ++j) {
        if ( ((j ^ m) & 1) != 0 ) {   /* j%2 != m%2 */
            if ((digit[j] & 1) == 0)  /* even digit → fail */
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, guard against misuse */

    idx_t count = 0;
    idx_t candidate = 1;
    for (;;) {
        if (has_property(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
