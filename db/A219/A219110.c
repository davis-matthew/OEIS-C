/*
 * A219110  Numbers for which at least one sum of two adjacent digits is not
 *          prime.
 * https://oeis.org/A219110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A219110: Numbers for which at least one sum of two adjacent
   decimal digits is not prime.
   The function scans integers in ascending order and returns the n-th
   number satisfying that property.
*/
#include <stdbool.h>

static const bool is_prime_adjacent[19] = {
    /*  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 */
    false, false, true, true, false, true, false, true, false, false, false, true, false, true, false, false, false, true, false
};

static bool has_nonprime_adjacent_sum(seq_t x)
{
    /* returns true if at least one adjacent digit sum is not prime */
    seq_t cur = x;
    int prevdigit = 0;
    bool first = true;
    while (cur > 0) {
        int digit = (int)(cur % 10);   /* cur is seq_t, % 10 works in C/C++ */
        cur = cur / 10;
        if (!first) {
            int sum = digit + prevdigit;
            if (!is_prime_adjacent[sum]) {
                return true;
            }
        }
        prevdigit = digit;
        first = false;
    }
    return false;   /* all sums were prime */
}

seq_t a(idx_t n)
{
    /* n >= 1; offset is 1, a(1) = 10 */
    idx_t found = 0;
    seq_t value = (seq_t)10;   /* start from the smallest two‑digit number */
    while (1) {
        if (has_nonprime_adjacent_sum(value)) {
            ++found;
            if (found == n) {
                return value;
            }
        }
        value = value + 1;   /* seq_t + 1 works for all supported seq_t types */
    }
}
