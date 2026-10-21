/*
 * A059707  If all digits have the same parity, stop; otherwise write down the
 *          number formed by the even digits and the number formed by the odd
 *          digits and multiply them; repeat.
 * https://oeis.org/A059707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: iterative digit‑parity decomposition
 * a(0) = 0
 * For n > 0: repeatedly
 *   if all digits of current x have the same parity (all even or all odd)
 *       stop and return x
 *   else
 *       let e be the number formed by the even digits (in original order)
 *       let o be the number formed by the odd digits (in original order)
 *       set x = e * o
 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* single‑digit numbers are already all same parity */
    if (x < (seq_t)10)
        return x;

    /* maximum number of decimal digits for a 64‑bit integer is 20 */
    enum { MAX_DIGITS = 20 };

    while (true) {
        /* Scan digits, collecting even and odd ones (in reverse). */
        bool seen_even = false, seen_odd = false;
        uint8_t even_rev[MAX_DIGITS];
        uint8_t odd_rev[MAX_DIGITS];
        int e_len = 0, o_len = 0;

        seq_t tmp = x;
        while (tmp > (seq_t)0) {
            uint8_t d = (uint8_t)(tmp % (seq_t)10);
            if ((d & 1) == 0) {            /* even digit */
                even_rev[e_len++] = d;
                seen_even = true;
            } else {                       /* odd digit */
                odd_rev[o_len++] = d;
                seen_odd = true;
            }
            tmp /= (seq_t)10;
        }

        /* If all digits have the same parity, we are done. */
        if (!(seen_even && seen_odd))
            return x;

        /* Build the even and odd numbers in correct order. */
        seq_t even_num = 0, odd_num = 0;
        for (int i = e_len - 1; i >= 0; --i)
            even_num = even_num * (seq_t)10 + even_rev[i];
        for (int i = o_len - 1; i >= 0; --i)
            odd_num = odd_num * (seq_t)10 + odd_rev[i];

        x = even_num * odd_num;
        /* Repeat with the new value. */
    }
}
