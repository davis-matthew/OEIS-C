/*
 * A036991  Numbers k with the property that in the binary expansion of k,
 *          reading from right to left, the number of 0's never exceeds the
 *          number of 1's.
 * https://oeis.org/A036991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th integer k such that, when its binary expansion
 *            is read from least significant bit to most significant bit, the
 *            number of 0's never exceeds the number of 1's
 * Range:     u8 n <= 79, u16 n <= 9832, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If a(n) = A000225(k) for some k, then a(n+1) = a(n) + A060546(k). */

#include <stdbool.h>

static bool valid_number(seq_t x)
{
    seq_t bal = 0;              /* current difference (ones – zeros)   */
    while (x > 0) {
        if (x & 1)              /* bit is 1 */
            bal++;
        else {                  /* bit is 0 */
            if (bal == 0)       /* would make zeros exceed ones   */
                return false;
            bal--;
        }
        x >>= 1;                /* move to next bit toward MSB    */
    }
    return true;                /* 0 is trivially valid            */
}

seq_t a(idx_t n)
{
    idx_t  seen  = 0;           /* how many valid numbers found   */
    seq_t  k     = 0;           /* current integer to test        */
    while (1) {
        if (valid_number(k)) {
            /* found a valid number  */
            seen++;
            if (seen == n)
                return k;
        }
        k++;                    /* try next integer               */
    }
}
