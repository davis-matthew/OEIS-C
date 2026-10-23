/*
 * A105716  Smallest m such that 6 is at the n-th position of the decimal
 *          representation of the m-th Fibonacci number.
 * https://oeis.org/A105716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   digit_len, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest Fibonacci number whose n‑th digit
   (counted from the right, offset 0) is 6.
   The sequence term is that Fibonacci number (a(n)=F_k). */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/seq_to_dec_string.h"

seq_t a(idx_t n)
{
    /* Fibonacci iteration: F0 = 0, F1 = 1. */
    seq_t cur = 0;          /* F_m   */
    seq_t next = 1;         /* F_{m+1} */

    /* Buffer to hold the decimal representation.
       20005 is adequate for n ≤ 10000 (max length ≈ 10000). */
    char buf[20005];

    for (;;) {
        /* Only inspect numbers that have at least n+1 digits. */
        idx_t len = digit_len(cur);
        if (len > n) {
            /* Convert to string and check the n-th digit from the right. */
            seq_to_dec_string(cur, buf, sizeof(buf));
            if (buf[len - 1 - n] == '6')
                return cur;
        }

        /* Advance one Fibonacci step: cur = F_{m+1}, next = F_{m+2}. */
        seq_t temp = cur;
        cur = next;
        next = temp + next;
    }
}
