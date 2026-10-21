/*
 * A219242  Distribution of letters in the name of all months of the year
 *          (English).
 * https://oeis.org/A219242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219242 – counts of each letter in the English month names.
   a(1) = number of times 'a' occurs, a(2) = 'b', …, a(26) = 'z'.
   The function initializes a static array of counts the first time it
   is called and returns the requested value.  If n is outside 1..26,
   0 is returned. */
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based result: a(1) for 'a', a(26) for 'z' */
    static bool initialized = false;
    static seq_t counts[26];

    if (!initialized)
    {
        static const char *const months[12] = {
            "January",   "February",  "March",
            "April",     "May",       "June",
            "July",      "August",    "September",
            "October",   "November",  "December"
        };
        const size_t month_cnt = 12;

        for (size_t mi = 0; mi < month_cnt; ++mi)
        {
            const char *p = months[mi];
            while (*p)
            {
                unsigned char ch = (unsigned char)*p;
                if (isalpha(ch))
                {
                    unsigned char lower = (unsigned char)tolower(ch);
                    unsigned idx = lower - 'a';        /* 0 .. 25 */
                    counts[idx] += (seq_t)1;
                }
                ++p;
            }
        }
        initialized = true;
    }

    if (n >= 1 && n <= 26)
        return counts[(idx_t)(n - 1)];
    else
        return (seq_t)0;
}
