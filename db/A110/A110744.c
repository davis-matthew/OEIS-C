/*
 * A110744  Integers k such that k divides LS(k) or LS(k) divides k, where LS
 *          is the "Look and Say" function (A045918).
 * https://oeis.org/A110744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th positive integer k such that k divides LS(k) or
 *            LS(k) divides k, where LS(k) is the look‑and‑say (run‑length
 *            encoding) of the decimal digits of k
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Look‑and‑Say of an unsigned 64‑bit integer.  Returns the result as
   an unsigned 64‑bit integer.  The output never exceeds 10^19 for
   all inputs that fit in 64 bits, so uint64_t suffices. */
static uint64_t look_and_say_uint64(uint64_t x)
{
    /* Store digits least significant first.  20 digits are enough
       for 64‑bit values. */
    int digits[20];
    int len = 0;
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x > 0) {
            digits[len++] = (int)(x % 10);
            x /= 10;
        }
    }

    uint64_t result = 0;
    for (int i = len - 1; i >= 0; ) {
        int d = digits[i];
        int count = 1;
        i--;
        while (i >= 0 && digits[i] == d) {
            count++;
            i--;
        }
        /* Append decimal representation of 'count'.  'count' is at most 20,
           therefore at most two decimal digits. */
        int cbuf[3];
        int clen = 0;
        int c = count;
        do {
            cbuf[clen++] = c % 10;
            c /= 10;
        } while (c > 0);
        for (int j = clen - 1; j >= 0; --j) {
            result = result * 10 + (uint64_t)cbuf[j];
        }
        /* Append the digit itself. */
        result = result * 10 + (uint64_t)d;
    }

    return result;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard against underflow */

    uint64_t found = 0;
    uint64_t k = 1;
    uint64_t last = 0;

    while (found < n) {
        uint64_t ls = look_and_say_uint64(k);
        if (ls % k == 0 || k % ls == 0) {
            last = k;
            found++;
        }
        k++;
    }
    return (seq_t)last;
}
