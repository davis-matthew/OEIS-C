/*
 * A033988  Write 0,1,2,... in a clockwise spiral on a square lattice, writing
 *          each digit at a separate lattice point, starting with 0 at the
 *          origin and 1 at x=0, y=-1; sequence gives the numbers on the
 *          positive y-axis.
 * https://oeis.org/A033988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 336, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033988: a(n) = A033307(4*n^2 + n - 1) for n > 0 */
/* Computes the digit at the positive y‑axis in a clockwise spiral of digits. */

#include <stdint.h>
#include <stdbool.h>

/* up to 10^18 for safety; only the first 19 entries are needed */
#define POW10_MAX 20
static const idx_t pow10[POW10_MAX] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

/* Count decimal digits of a non‑negative integer (1 for 0). */
static inline idx_t digit_count_idx(idx_t n)
{
    if (n == 0) return 1;
    idx_t cnt = 0;
    while (n) {
        n /= 10;
        ++cnt;
    }
    return cnt;
}

/* Total digits in 0,1,…,m inclusive. */
static idx_t cumulative_digits(idx_t m)
{
    if (m < 10) return m + 1;               /* 0‑9 */
    idx_t d = digit_count_idx(m);
    idx_t low = pow10[d - 1];
    idx_t sum = 0;
    for (idx_t i = 1; i < d; ++i) {
        idx_t count = (i == 1) ? 10ULL : 9ULL * pow10[i - 1];
        sum += i * count;
    }
    sum += d * (m - low + 1);
    return sum;
}

/* Find smallest m with cumulative_digits(m) > target (0‑based). */
static idx_t find_number_with_digit(idx_t target)
{
    idx_t lo = 0, hi = 1;
    while (cumulative_digits(hi) <= target) hi <<= 1; /* raise until high enough */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cumulative_digits(mid) > target)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

/* Spiral index (0‑based) of point (x,y) in the defined spiral. */
static inline idx_t spiral_index(idx_t x, idx_t y)
{
    int64_t ix = (int64_t)x;
    int64_t iy = (int64_t)y;
    int64_t ax = ix >= 0 ? ix : -ix;
    int64_t ay = iy >= 0 ? iy : -iy;
    int64_t r = ax > ay ? ax : ay;
    if (r == 0) return 0;                     /* origin */

    idx_t base = (2 * (idx_t)r - 1);
    base *= base;                              /* (2r-1)^2 */

    idx_t offset;
    if (iy == -(int64_t)r) {                  /* bottom side */
        offset = (idx_t)((int64_t)(r - 1) - ix);
    } else if (ix == -(int64_t)r) {            /* left side */
        offset = (idx_t)(2 * r + (iy - (-(int64_t)r + 1)));
    } else if (iy == (int64_t)r) {             /* top side */
        offset = (idx_t)(4 * r + (ix - (-(int64_t)r + 1)));
    } else {                                  /* right side */
        int64_t t = (int64_t)(r - 1) - iy;     /* y decreases */
        offset = (idx_t)(6 * r + t);
    }
    return base + offset;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* a(0) = 0 */

    /* step index of the lattice point (0,n) in the spiral */
    idx_t s = spiral_index(0, n);              /* 0‑based digit index */

    /* locate the number containing this digit */
    idx_t m = find_number_with_digit(s);       /* smallest m with cum > s */
    idx_t cum_before = cumulative_digits(m - 1);
    idx_t offset = s - cum_before;             /* 0‑based offset inside m */

    idx_t d = digit_count_idx(m);               /* number of decimal digits */
    idx_t idx_from_right = d - 1 - offset;      /* position from right end */
    seq_t digit = ( (seq_t)m / (seq_t)pow10[idx_from_right] ) % (seq_t)10;

    return digit;
}
