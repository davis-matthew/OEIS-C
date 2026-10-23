/*
 * A153839  First Sunday in n-th month of 365-day year starting on Sunday.
 * https://oeis.org/A153839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = ((7 - ((sum_{i=1}^{n-1} month[i]) mod 7)) mod 7) + 1
 *            where month[1..12] = {31,28,31,30,31,30,31,31,30,31,30,31}
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Month lengths for a non‑leap 365‑day year */
    static const seq_t month_len[12] =
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    /* Store the computed first‑Sunday dates for each month
       (initialised only once) */
    static seq_t first_sunday[12];
    static bool init = false;

    if (!init) {
        seq_t offset = 0;  /* weekday offset of the first day of the month,
                              0 = Sunday, 1 = Monday, …, 6 = Saturday */
        for (idx_t i = 0; i < 12; ++i) {
            /* first Sunday of this month */
            first_sunday[i] = ((7 - (offset % 7)) % 7) + 1;
            /* compute offset for next month */
            offset = (offset + month_len[i]) % 7;
        }
        init = true;
    }

    /* Months repeat every 12; sequence is periodic with period 12 */
    idx_t k = (n - 1) % 12;
    return first_sunday[k];
}
