/*
 * A031067  Write the (n+1)st Fibonacci number in base 8 and juxtapose.
 * https://oeis.org/A031067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = the n-th digit obtained by concatenating * the base‑8
 *            representations of Fibonacci numbers * F_k (k ≥ 2). * The digits
 *            are written most significant first. * Example: F₂=1→"1",
 *            F₃=2→"2", F₄=3→"3", F₅=5→"5", * F₆=8→"10", F₇=13→"15", … →
 *            1,2,3,5,1,0,1,5,
 * Range:     u8 n <= 23, u16 n <= 75, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if ((idx_t)n == 0) return 0;          /* safety for n < 1 */

    /* Fibonacci iteration starting with F₂=1. */
    seq_t prev = 0;   /* F₀ */
    seq_t cur  = 1;   /* F₁ */

    idx_t cum_digits = 0;                /* digits seen so far */

    /* Large enough buffer for the digits of a Fibonacci number.     */
    /* For n up to 10000 the largest number we handle has < 30000    */
    /* octal digits, so 200000 is safely overkill.                   */
    static char digit_buf[200000];

    /* We will generate Fibonacci numbers until we pass digit n.    */
    for (idx_t k = 2;; ++k) {
        /* Next Fibonacci number: F_k = prev + cur. */
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;

        /* Convert next to base‑8 digits, LSB first. */
        seq_t temp = next;
        idx_t len = 0;
        while (temp != 0) {
            seq_t rem = temp % 8;                 /* remainder 0‑7 */
            digit_buf[len++] = (char)(int)rem;    /* store LSB first */
            temp = temp / 8;
        }

        /* If the next number were 0 (which never happens here), */
        /* it would contribute a single digit ‘0’.              */
        if (len == 0) len = 1, digit_buf[0] = 0;

        /* Check whether the desired digit lies in this number. */
        if (cum_digits + len < n) {
            cum_digits += len;
            continue;
        } else {
            /* Position of the desired digit within this number
               (1‑based from the most significant digit).      */
            idx_t pos_in_number = n - cum_digits;            /* 1‑based */
            idx_t target_index = len - pos_in_number;        /* 0‑based LSB‑first */
            seq_t result = (seq_t)digit_buf[target_index];
            return result;
        }
    }
}
