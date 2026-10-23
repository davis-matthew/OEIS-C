/*
 * A153842  First Sunday in n-th month of 365-day year starting on Wednesday.
 * https://oeis.org/A153842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* First Sunday of the n‑th month in a 365‑day year that starts on Wednesday. */
#include <stdint.h>

static const int month_len[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, guard against overflow */

    /* month index within a 12‑month cycle (0‑based) */
    uint64_t month = (uint64_t)(n - 1) % 12ULL;

    /* total days from the year start up to the first day of this month */
    int shift = 0;
    for (int i = 0; i < month; ++i) {
        shift += month_len[i];
    }
    shift %= 7;

    /* day_of_week: 0=Mon, 1=Tue, 2=Wed, 3=Thu, 4=Fri, 5=Sat, 6=Sun */
    int first_day = (2 + shift) % 7;          /* Wednesday (2) + shift */

    /* offset from the first day of the month to the first Sunday */
    int offset = (6 - first_day + 7) % 7;     /* 6 represents Sunday */

    return (seq_t)1 + (seq_t)offset;
}
