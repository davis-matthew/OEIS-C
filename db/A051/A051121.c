/*
 * A051121  Dates at fortnightly intervals starting on Jan 01 of a leap year
 *          in the Julian calendar.
 * https://oeis.org/A051121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = day of month of the nth 14‑day date starting from Jan 01
 *            of a Julian leap year
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool built = false;
static seq_t dates[1461];

/* Month lengths for a normal and a leap year in the Julian calendar. */
static const seq_t monthNorm[12] = {31, 28, 31, 30, 31, 30,
                                    31, 31, 30, 31, 30, 31};
static const seq_t monthLeap[12] = {31, 29, 31, 30, 31, 30,
                                    31, 31, 30, 31, 30, 31};

static void build_dates(void)
{
    /* Build the table once. It contains for each of the 1461 days
       in a 4‑year Julian cycle the day of month. */
    int pos = 0;
    for (int y = 0; y < 4; ++y) {
        const seq_t *month = (y == 0) ? monthLeap : monthNorm;
        for (int m = 0; m < 12; ++m) {
            seq_t days = month[m];
            for (seq_t d = 1; d <= days; ++d) {
                dates[pos++] = d;
            }
        }
    }
    /* pos should be 1461 */
    (void)pos;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; compute the day offset within the 1461‑day period. */
    idx_t dayIndex = (14ULL * (n - 1)) % 1461ULL;

    if (!built) {
        /* initialise once; static guarantee that this is thread safe in
           single-threaded contexts, which is the requirement for the
           competition environment. */
        build_dates();
        built = true;
    }

    return dates[dayIndex];
}
