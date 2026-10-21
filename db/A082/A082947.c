/*
 * A082947  Palindromes not divisible by any of their digits.
 * https://oeis.org/A082947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pal_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates palindromes in increasing order and returns the nth
   palindrome that is not divisible by any of its decimal digits
   (digits 0 or 1 automatically fail). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pal_ok.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* offset n>=1, but guard */

    idx_t wanted = n;
    idx_t found  = 0;

    for (int len = 1; ; ++len) {
        int prefLen = (len % 2) ? (len / 2 + 1) : (len / 2);
        uint64_t start = 1;
        for (int i = 1; i < prefLen; ++i)
            start *= 10;                  /* first digit 1-9 */
        uint64_t end = start * 10 - 1;    /* inclusive upper bound */

        for (uint64_t pref = start; pref <= end; ++pref) {
            seq_t pal = (seq_t)pref;
            uint64_t mirror = pref;
            if (len % 2)               /* skip middle digit for odd length */
                mirror /= 10;

            while (mirror > 0) {
                pal = pal * 10 + (seq_t)(mirror % 10);
                mirror /= 10;
            }

            if (pal_ok(pal)) {
                ++found;
                if (found == wanted)
                    return pal;
            }
        }
    }
}
