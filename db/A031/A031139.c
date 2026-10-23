/*
 * A031139  Number of letters in English words for months of year.
 * https://oeis.org/A031139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = number of letters in the English word for the n‑th month
 *            (a(1)=7 for January, a(2)=8 for February, … a(12)=8 for
 *            December). The sequence is periodic with period 12
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Names of the months in order.  The number of letters in each name is the
   value of the sequence for that month. */
static const char * const month_names[12] = {
    "January",   /* 7 */
    "February",  /* 8 */
    "March",     /* 5 */
    "April",     /* 5 */
    "May",       /* 3 */
    "June",      /* 4 */
    "July",      /* 4 */
    "August",    /* 6 */
    "September", /* 9 */
    "October",   /* 7 */
    "November",  /* 8 */
    "December"   /* 8 */
};

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  If n is 0 or less (which cannot
       happen with an unsigned type) we return 0. */
    if (n == 0) return (seq_t)0;

    /* Convert to 0‑based index for the month.  The modulo operation
       guarantees a value in [0,11] for any n ≥ 1. */
    idx_t month_index = (n - 1) % 12;
    const char *name = month_names[month_index];

    /* Count the number of letters in the month name.  The names contain
       only alphabetic characters, so simply iterate until the terminating
       null byte.  Use seq_t to hold the length. */
    seq_t len = 0;
    while (*name) {
        len++;
        name++;
    }
    return len;
}
