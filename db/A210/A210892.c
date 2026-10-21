/*
 * A210892  Dates after Jan 01 00 in chronological order which are palindromic
 *          when they are written in the format D.M.YY. The terms are listed
 *          as numbers. Leading zeros of the terms are suppressed.
 * https://oeis.org/A210892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 214, u64 n <= 214, u128 n <= 214,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A210892: palindromic dates in D.M.YY format after Jan 01 00.
   The function generates dates from 01.01.00 to 31.12.99, collects
   those whose D.M.YY string (including dots) is a palindrome, stores
   them in a static array, and returns the n‑th term (1‑based). */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t cache[256];
    static size_t cache_len = 0;

    if (cache_len == 0) {
        for (int year = 0; year <= 99; ++year) {
            int leap = (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
            for (int month = 1; month <= 12; ++month) {
                int days;
                switch (month) {
                    case 1: case 3: case 5: case 7: case 8: case 10: case 12: days = 31; break;
                    case 4: case 6: case 9: case 11: days = 30; break;
                    case 2: days = leap ? 29 : 28; break;
                    default: days = 0; break;
                }
                for (int day = 1; day <= days; ++day) {
                    char s[32];
                    int pos = 0;
                    if (day >= 10) {
                        s[pos++] = '0' + day / 10;
                        s[pos++] = '0' + day % 10;
                    } else {
                        s[pos++] = '0' + day;
                    }
                    s[pos++] = '.';
                    if (month >= 10) {
                        s[pos++] = '0' + month / 10;
                        s[pos++] = '0' + month % 10;
                    } else {
                        s[pos++] = '0' + month;
                    }
                    s[pos++] = '.';
                    int yy = year % 100;
                    s[pos++] = '0' + yy / 10;
                    s[pos++] = '0' + yy % 10;
                    int len = pos;

                    int pal = 1;
                    for (int i = 0; i < len / 2; ++i) {
                        if (s[i] != s[len - 1 - i]) { pal = 0; break; }
                    }

                    if (pal) {
                        int lenM = (month < 10) ? 1 : 2;
                        seq_t val;
                        if (lenM == 1)
                            val = (seq_t)(day * 1000 + month * 100 + yy);
                        else
                            val = (seq_t)(day * 10000 + month * 100 + yy);
                        cache[cache_len++] = val;
                    }
                }
            }
        }
    }

    if (n == 0 || (size_t)(n - 1) >= cache_len) {
        /* Out of range – defined behavior is unspecified; return 0. */
        return (seq_t)0;
    }
    return cache[(size_t)(n - 1)];
}
