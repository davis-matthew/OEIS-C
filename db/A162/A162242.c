/*
 * A162242  Years in which January first falls on a Friday (since 2001).
 * https://oeis.org/A162242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th year >= 2001 whose January 1 falls on Friday. */
/* Wednesday offset: 0 = Monday, ..., 4 = Friday.  Start from 2001 (Monday). */
seq_t a(idx_t n)
{
    idx_t year  = 2001;  /* current year being examined   */
    idx_t count = 0;     /* number of qualifying years seen */
    int   day   = 0;     /* weekday of Jan 1 of 'year', 0 = Monday */

    for (;;)
    {
        /* If January 1 of the current year is Friday, record it. */
        if (day == 4) {
            ++count;
            if (count == n) {
                return (seq_t)year;
            }
        }

        /* Advance to Jan 1 of the next year.  Add 1 or 2 days
           depending on whether the current year is a leap year. */
        if ( (year % 400u) == 0u || ( (year % 4u) == 0u && (year % 100u) != 0u ) ) {
            day = (day + 2) % 7;
        } else {
            day = (day + 1) % 7;
        }

        ++year;
    }
}
