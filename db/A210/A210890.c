/*
 * A210890  Dates after Jan 01 00 in chronological order which are palindromic
 *          when they are written in the format D.M.YY. The terms are listed
 *          as numbers (without the dots).
 * https://oeis.org/A210890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 322, u64 n <= 322, u128 n <= 322,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth palindromic date in D.M.YY format between Mar 01 00
   and Dec 31 99 (inclusive).  The dates are enumerated in chronological
   order.  This implementation constructs the list on the first call
   and then indexes it. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: leap year test for Gregorian calendar. */
static bool is_leap_year(int year)
{
    return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

/* Helper: number of days in a month, accounting for leap years. */
static int month_days(int month, int year)
{
    static const int md[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && is_leap_year(year))
        return 29;
    return md[month - 1];
}

/* Static cache of palindromic dates (maximum required is 322). */
static bool cache_init = false;
static uint32_t cache[400];
static size_t cache_len;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Build cache on first call. */
    if (!cache_init) {
        const uint32_t POW10[3] = {1, 10, 100}; /* POW10[i] = 10^i */
        size_t cnt = 0;

        for (int yr = 2000; yr < 2100; ++yr) {            /* yyyy 2000‑2099 */
            for (int month = 1; month <= 12; ++month) {
                if (yr == 2000 && month < 3)             /* before Mar 1 00 */
                    continue;
                int days = month_days(month, yr);
                for (int day = 1; day <= days; ++day) {
                    uint32_t num = (uint32_t)day;
                    int mlen = (month < 10) ? 1 : 2;
                    num = num * POW10[mlen] + (uint32_t)month;
                    num = num * 100 + (uint32_t)(yr % 100);      /* concatenate YY */
                    /* test palindrome */
                    uint32_t rev = 0, t = num;
                    while (t > 0) {
                        rev = rev * 10 + (t % 10);
                        t /= 10;
                    }
                    if (rev == num) {
                        cache[cnt++] = num;
                    }
                }
            }
        }
        cache_len = cnt;
        cache_init = true;
    }

    if ((size_t)(n - 1) >= cache_len)
        return (seq_t)0;      /* out of range */

    return (seq_t)cache[n - 1];
}
