/*
 * A023059  Positive numbers k such that k and 2*k are anagrams in base 4
 *          (written in base 4).
 * https://oeis.org/A023059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k and 2*k are anagrams in base 4. */
#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term of A023059 (1‑based index). */
seq_t a(idx_t n)
{
    /* Counters for how many times each base‑4 digit appears. */
    int cnt_x[4], cnt_y[4];
    /* Temporary storage for base‑4 digits (reversed). */
    uint8_t digits[64];        /* enough for < 2^63 (max 32 digits) */

    idx_t found = 0;              /* how many values found so far */
    uint64_t x = 1;               /* candidate base‑4 value (in decimal) */

    while (true) {
        /* ----- digits of x, and their counts ----- */
        for (int d = 0; d < 4; ++d) cnt_x[d] = 0;
        int len = 0;
        uint64_t tmp = x;
        /* x > 0 always here */
        while (tmp > 0) {
            uint8_t d = tmp & 3;          /* tmp % 4 */
            cnt_x[d]++;
            digits[len++] = d;             /* store reversed */
            tmp >>= 2;                     /* tmp /= 4 */
        }

        /* ----- digits of 2*x (the doubled value) ----- */
        uint64_t y = x << 1;
        for (int d = 0; d < 4; ++d) cnt_y[d] = 0;
        tmp = y;
        while (tmp > 0) {
            uint8_t d = tmp & 3;
            cnt_y[d]++;
            tmp >>= 2;
        }

        /* ----- compare digit multisets ----- */
        bool ok = true;
        for (int d = 0; d < 4; ++d)
            if (cnt_x[d] != cnt_y[d]) { ok = false; break; }

        if (ok) {
            /* Convert the base‑4 digits to the decimal integer that
               represents the string of those digits. */
            seq_t val = 0;
            for (int i = len - 1; i >= 0; --i)
                val = val * 10 + (seq_t)digits[i];

            ++found;
            if (found == n) return val;
        }

        /* Next candidate value. */
        ++x;
    }
}
