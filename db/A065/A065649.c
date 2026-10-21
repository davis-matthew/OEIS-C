/*
 * A065649  Permutation of nonnegative integers based on Champernowne's
 *          constant 0.123456789101112131415...
 * https://oeis.org/A065649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nice, nonn, look
 * Formula:   a(n) = if n = 0 then 0 else 10*(A065648(n)-1) + A033307(n-1)
 * Range:     u8 n <= 204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function simulates the permutation by counting the occurrences of each
   digit of Champernowne's constant (0.123456789101112…) and using the
   occurrence number to determine the tens digit.  A first-occurrence counter
   of 0 is initialized to 1 to account for the leading zero of the integer
   part of the constant. */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t pos = n - 1;               /* 0‑based index in Champernowne digits */
    uint64_t counter[10] = {1,0,0,0,0,0,0,0,0,0};  /* start counter[0]=1 */

    idx_t current_pos = 0;
    int digit_at_pos = -1;
    int high_at_pos = 0;
    bool found = false;

    for (idx_t num = 1; !found; ++num) {
        char buf[32];
        int len = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)num);
        for (int i = 0; i < len; ++i) {
            int d = buf[i] - '0';
            counter[d]++;                       /* increment occurrence count */

            if (current_pos == pos) {
                digit_at_pos = d;
                high_at_pos = (int)(counter[d] - 1);
                found = true;
                break;
            }
            ++current_pos;
        }
    }

    seq_t result = (seq_t)high_at_pos * (seq_t)10 + (seq_t)digit_at_pos;
    return result;
}
