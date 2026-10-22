/*
 * A003786  Day-of-month at weekly intervals from Jan 01 of a leap year (a
 *          finite sequence with 53 terms).
 * https://oeis.org/A003786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = day_of_month( 1 + (n-1)*7 ) for days in a leap year
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int days_in_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    idx_t day_of_year = 1 + (n - 1) * 7;
    idx_t cumulative_days = 0;
    
    for (int month = 0; month < 12; month++) {
        idx_t days_this_month = (idx_t)days_in_month[month];
        if (day_of_year <= cumulative_days + days_this_month) {
            return (seq_t)(day_of_year - cumulative_days);
        }
        cumulative_days += days_this_month;
    }
    
    return 0;
}
