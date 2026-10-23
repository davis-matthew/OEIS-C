/*
 * A184989  Digits of n and of n-1 interleaved in decimal representation.
 * https://oeis.org/A184989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the integer obtained by interleaving the decimal digits of n and n‑1,
   starting with the most significant digit of n.  If one number has more
   digits, the remaining digits are appended. */
#include <stdint.h>
#include <stddef.h>

#define MAX_DIGITS 22   /* enough for 64‑bit integers (≤20 decimal digits) */

static void get_digits_rev(idx_t x, uint8_t *buf, size_t *len) {
    if (x == 0) {                 /* treat 0 as a single digit '0' */
        *len = 1;
        buf[0] = 0;
        return;
    }
    size_t i = 0;
    while (x > 0) {
        buf[i++] = (uint8_t)(x % 10);
        x /= 10;
    }
    *len = i;                     /* least‑significant first */
}

seq_t a(idx_t n) {
    idx_t m = n - 1;              /* n‑1, valid because n≥1 */
    uint8_t buf_n[MAX_DIGITS], buf_m[MAX_DIGITS];
    size_t len_n, len_m;

    get_digits_rev(n, buf_n, &len_n);
    get_digits_rev(m, buf_m, &len_m);

    seq_t res = 0;
    size_t max_len = len_n > len_m ? len_n : len_m;

    for (size_t i = 0; i < max_len; ++i) {
        if (len_n > i) {
            res = res * 10 + (seq_t)buf_n[len_n - 1 - i];
        }
        if (len_m > i) {
            res = res * 10 + (seq_t)buf_m[len_m - 1 - i];
        }
    }
    return res;
}
