/*
 * A002651  Dates at 16-day intervals starting at the beginning of a leap
 *          year.
 * https://oeis.org/A002651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   day-of-month of day number 16n (1-based) counted from Jan 1 of a
 *            leap year, following the Julian 4-year cycle (366+365+365+365 =
 *            1461 days), period 1461 in n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned long long r = (unsigned long long)(n % 1461);
    unsigned long long d = (16ULL * r + 1461ULL - 1ULL) % 1461ULL; /* 0-based day in cycle */
    int leap;
    if (d < 366) {
        leap = 1;
    } else {
        d -= 366;
        d %= 365;
        leap = 0;
    }
    for (int m = 0; m < 12; m++) {
        unsigned long long len = (unsigned long long)mdays[m] + ((m == 1 && leap) ? 1ULL : 0ULL);
        if (d < len) return (seq_t)(d + 1);
        d -= len;
    }
    return (seq_t)1;
}
