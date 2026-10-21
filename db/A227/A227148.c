/*
 * A227148  Numbers k for which the sum of digits is even when k is written in
 *          the factorial base (A007623).
 * https://oeis.org/A227148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 2520, u32 n <= 2520, u64 n <= 2520,
 *            u128 n <= 2520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth non‑negative integer whose factorial‑base digit sum is even.
   The function searches for the desired value by testing successive integers
   and computing the parity of their factorial‑base digit sum. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    for (seq_t k = 0; ; ++k) {
        seq_t temp = k;
        int parity = 0;
        int i = 1;
        while (temp) {
            seq_t base = (seq_t)(i + 1);          /* next factorial base (i+1) */
            seq_t digit = temp % base;            /* digit at this position  */
            parity ^= (int)(digit & 1);           /* toggle parity if digit odd */
            temp /= base;
            ++i;
        }
        if (parity == 0) {                        /* even digit sum -> include */
            ++count;
            if ((idx_t)count == n)
                return k;
        }
    }
}
