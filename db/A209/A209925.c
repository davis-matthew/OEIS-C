/*
 * A209925  ASCII encoded concatenation of the months' names, spelled in
 *          English.
 * https://oeis.org/A209925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209925
 * a(n) is the ASCII code of the (n-1)-th character of the infinite
 * repetition of the English month names: January February March ...
 * (each month name appears once per cycle, then the sequence repeats).
 */

#include <stddef.h>

static const char * const month_names[12] = {
    "January",  "February", "March",    "April",
    "May",      "June",     "July",     "August",
    "September","October",  "November","December"
};

static const unsigned char month_len[12] = {
    7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* offset 1 is the first term */
    idx_t offset = (n - 1) % 74;            /* 74 = total characters in one year */
    int month = 0;
    while (offset >= month_len[month]) {
        offset -= month_len[month];
        ++month;
    }
    const char *s = month_names[month];
    return (seq_t)(unsigned char)s[(size_t)offset];
}
