/*
 * A224940  Non-leap years beginning and ending on Tuesday.
 * https://oeis.org/A224940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224940: Non‑leap years beginning and ending on Tuesday.
   The Gregorian calendar repeats every 400 years. In each 400‑year
   cycle there are exactly 44 such years.  We pre‑compute the 44
   offsets within one 400‑year block starting at 1585 and then
   translate the requested index n into the appropriate cycle. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑computed offsets (in years) from the first block start (1585). */
    static seq_t offsets[44];
    static size_t offset_count = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t start_year = 1585;
        const idx_t end_year   = start_year + 400;          /* [1585,1984] */

        /* t array for Sakamoto's algorithm (month offsets). */
        static const idx_t month_table[12] = {0,3,2,5,0,3,5,1,4,6,2,4};

        for (idx_t year = start_year; year < end_year; ++year) {
            /* Gregorian leap test */
            bool leap = ((year % 4 == 0 && year % 100 != 0) ||
                         (year % 400 == 0));
            if (leap)
                continue;           /* skip leap years */

            /* Day of week for Jan 1 of the current year   */
            idx_t y = year;
            idx_t m = 1;          /* January */
            idx_t d = 1;          /* first day */

            if (m < 3)
                y -= 1;

            idx_t w = (y + y / 4 - y / 100 + y / 400 +
                       month_table[m - 1] + d) % 7;

            if (w == 2) {         /* Tuesday (0=Sun,1=Mon,2=Tue) */
                offsets[offset_count++] = year - start_year;
            }
        }
        initialized = true; /* offsets[0] will be 0 for year 1585 */
    }

    /* Number of years per 400‑year cycle that satisfy the condition. */
    const idx_t cycle_length = (idx_t)offset_count; /* should be 44 */

    /* Convert the 1‑based index n into 0‑based positions. */
    idx_t idx = n - 1;
    idx_t pos   = idx % cycle_length;
    idx_t cycle = idx / cycle_length;

    /* Build the answer. */
    seq_t result =
        (seq_t)1585 + offsets[(size_t)pos] + (seq_t)400 * cycle;

    return result;
}
