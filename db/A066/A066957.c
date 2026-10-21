/*
 * A066957  a(n) = number of ways of placing '+' and '-' among the digits
 *          123456789 so that the result of the expression is n, '-' before 1
 *          IS allowed.
 * https://oeis.org/A066957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ways of placing '+' or '-' between the digits
   1 2 3 4 5 6 7 8 9 (or concatenating them) with an optional leading '-'
   so that the resulting expression equals n. */

#include <stdint.h>

static const int pow3[8] = {1, 3, 9, 27, 81, 243, 729, 2187};

seq_t a(idx_t n)
{
    /* 123456789 is the maximum absolute value that can be obtained. */
    if ((long long)n > 123456789)
        return 0;

    seq_t count = 0;
    const int totalMasks = 6561;   /* 3^8 */
    for (int first_sign = 0; first_sign < 2; ++first_sign) {
        const long long signFirst = first_sign ? -1LL : 1LL;
        for (int mask = 0; mask < totalMasks; ++mask) {
            long long sum = 0;
            long long num = 1;            /* start with digit 1 */
            long long sign = signFirst;   /* sign for the current group */
            int m = mask;
            for (int i = 0; i < 8; ++i) {
                int choice = m % 3;        /* 0: concatenate, 1: '+', 2: '-' */
                m /= 3;
                if (choice == 0) {
                    /* concatenate digit (i+2) to current number */
                    num = num * 10 + (i + 2);
                } else {
                    /* close current group with previous sign */
                    sum += sign * num;
                    /* start new group with digit (i+2) */
                    num = (i + 2);
                    /* set sign for the new group */
                    sign = (choice == 1) ? 1LL : -1LL;
                }
            }
            /* add the last group */
            sum += sign * num;
            if (sum == (long long)n)
                ++count;
        }
    }
    return count;
}
