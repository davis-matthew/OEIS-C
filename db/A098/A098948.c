/*
 * A098948  Numbers where 7 is the only odd decimal digit.
 * https://oeis.org/A098948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth positive integer whose decimal representation
 *            contains only the digits 0,2,4,6,7,8 and at least one digit 7
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *table = NULL;
    static idx_t len = 0;
    static size_t cap = 0;
    static seq_t last = (seq_t)0;

    /* safety for 0 (not used for n>=1) */
    if (n == 0) return (seq_t)0;

    /* generate terms until we have the nth term */
    while (len < n) {
        /* next candidate integer */
        last += (seq_t)1;

        /* test whether all digits are in {0,2,4,6,7,8} and
           whether at least one 7 is present */
        seq_t temp = last;
        bool ok = true;
        bool has7 = false;
        while (temp != (seq_t)0) {
            seq_t d = temp % (seq_t)10;          /* decimal digit */
            if (d == (seq_t)7) {
                has7 = true;
            } else if ((d % (seq_t)2) != (seq_t)0) {
                /* odd digit other than 7 */
                ok = false;
                break;
            }
            temp /= (seq_t)10;
        }

        if (ok && has7) {
            if (len == cap) {
                size_t newcap = (cap == 0 ? 16 : cap << 1);
                table = (seq_t *)realloc(table, newcap * sizeof(seq_t));
                cap = newcap;
            }
            table[len] = last;
            ++len;
        }
    }

    return table[n - 1];
}
