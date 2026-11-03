/*
 * A224944  Non-leap years beginning and ending on Saturday.
 * https://oeis.org/A224944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A224944: Non‑leap years that begin and end on Saturday.
 *
 * The Gregorian calendar repeats every 400 years.  In each 400‑year
 * cycle there are exactly 43 such years.  Let
 *
 *   offsets[i] = years[i] - 1583   (0 ≤ i < 43)
 *
 * be the 43 offsets within the cycle starting at year 1583.
 *
 * Then for any index n ≥ 1
 *
 *   a(n) = 1583
 *          + 400 * floor((n-1)/43)
 *          + offsets[(n-1) % 43]
 *
 * The offsets are computed once on the first call by testing each year
 * in the 400‑year window 1583 … 1982.  A year qualifies if it is
 * non‑leap and its 1st day of the year is a Saturday.
 *
 * The weekday of 1‑Jan‑Y is obtained by Zeller’s algorithm (Sakamoto's
 * variant) which uses only integer arithmetic.
 */

#include <stdint.h>

static int is_non_leap_year(idx_t Y)
{
    uint64_t y = (uint64_t)Y;
    return !((y % 400ULL == 0ULL) || ((y % 4ULL == 0ULL) && (y % 100ULL != 0ULL)));
}

static int weekday_gregorian_1jan(idx_t Y)
{
    static const int t[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    uint64_t y = (uint64_t)Y;
    /* month = 1, so for Jan and Feb, subtract 1 from year */
    y--;  /* year for formula */
    uint64_t w = (y + y / 4 - y / 100 + y / 400 + t[0] + 1) % 7;
    /* 0=Sunday, 6=Saturday */
    return (int)w;
}

seq_t a(idx_t n)
{
    /* static table and flag */
    static seq_t offsets[43];
    static int initialized = 0;

    if (!initialized) {
        int count = 0;
        const idx_t start_year = 1583;
        for (idx_t y = start_year; y <= start_year + 399; ++y) {
            if (is_non_leap_year(y) && weekday_gregorian_1jan(y) == 6) {
                offsets[count++] = (seq_t)(y - start_year);
            }
        }
        initialized = 1;
    }

    /* work in idx_t first, then convert to seq_t */
    idx_t idx = n - 1;
    idx_t cycle = idx / 43;
    idx_t pos   = idx % 43;

    seq_t result = (seq_t)1583;
    result += offsets[pos];
    /* multiply cycle by 400 in seq_t to avoid overflow in idx_t */
    result += (seq_t)400 * (seq_t)cycle;

    return result;
}
