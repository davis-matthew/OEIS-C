/*
 * A053456  Open disk numbers (version 1): a(n) is the number of points (i,j),
 *          i,j, integers, contained in an open disk of diameter n, centered
 *          at (0,0).
 * https://oeis.org/A053456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{(i,j)∈ℤ² : 4(i²+j²) < n²}|
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (open disk of diameter n centered at (0,0)) */

#include <stdint.h>
#include <math.h>

/* Integer square root for 128‑bit unsigned values */
static inline unsigned int isqrt128 (unsigned __int128 x) {
    unsigned long long r = sqrtl ((long double) x);
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) r++;
    while ((unsigned __int128)r * r > x) r--;
    return (unsigned int) r;
}

seq_t a (idx_t n) {
    if (n == 0) return 0;          /* no points in zero radius */

    /* n²  (fits in 128 bits for all practical n) */
    unsigned __int128 n2 = (unsigned __int128) n * n;

    /* condition 4(i² + j²) ≤ n² – 1 (strict < n²) */
    unsigned __int128 limit = n2 - 1;

    /* maximum |i| */
    unsigned int max_i = 0;
    for (;;) {
        unsigned __int128 val = (unsigned __int128)4 * max_i * max_i;
        if (val > limit) break;
        ++max_i;
    }
    if (max_i > 0) --max_i;        /* last valid i */

    seq_t total = 0;

    for (unsigned int i = 0; i <= max_i; ++i) {
        unsigned __int128 rem = limit - (unsigned __int128)4 * i * i;
        unsigned __int128 div4 = rem / 4;          /* floor division */
        unsigned int jmax = isqrt128 (div4);      /* floor sqrt(rem/4) */
        seq_t count_per_i = (seq_t)(2 * jmax + 1);
        if (i == 0)
            total += count_per_i;
        else
            total += 2 * count_per_i;             /* ±i symmetry */
    }

    return total;
}
