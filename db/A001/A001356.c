/*
 * A001356  Dates at fortnightly intervals from Jan 01 in the Julian calendar.
 * https://oeis.org/A001356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = day of month for the ((n-1)*14 + 1) th day in a 4-year
 *            Julian cycle (3 non-leap years + 1 leap year, period 1461)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int days_in_month_nonleap[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    static const int days_in_month_leap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Calculate which day in the 4-year cycle (1-indexed)
    uint64_t day_in_cycle = (((n - 1) % 1461) * 14 % 1461) + 1;
    
    // Determine which year in the cycle and which day in that year
    int year_in_cycle;
    uint64_t day_in_year;
    
    if (day_in_cycle <= 365) {
        year_in_cycle = 0;
        day_in_year = day_in_cycle;
    } else if (day_in_cycle <= 730) {
        year_in_cycle = 1;
        day_in_year = day_in_cycle - 365;
    } else if (day_in_cycle <= 1095) {
        year_in_cycle = 2;
        day_in_year = day_in_cycle - 730;
    } else {
        year_in_cycle = 3;
        day_in_year = day_in_cycle - 1095;
    }
    
    const int *days_in_month = (year_in_cycle == 3) ? days_in_month_leap : days_in_month_nonleap;
    
    // Find which month and day of month
    uint64_t cumulative_days = 0;
    for (int month = 0; month < 12; month++) {
        if (cumulative_days + days_in_month[month] >= day_in_year) {
            return (seq_t)(day_in_year - cumulative_days);
        }
        cumulative_days += days_in_month[month];
    }
    
    return 31;
}
