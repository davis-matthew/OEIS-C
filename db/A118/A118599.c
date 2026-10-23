/*
 * A118599  Palindromes in base 8 (written in base 8).
 * https://oeis.org/A118599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence of palindromes in base 8, written in base 8
   (the returned number is the decimal value of the base‑8 digit string) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n<1, safe return */

    enum { MAX_HALF = 20 };
    idx_t pow8[MAX_HALF];
    pow8[0] = (idx_t)1;

    idx_t cumulative = 0;                /* number of palindromes counted so far */
    int    L = 1;                         /* current length being considered */

    while (1) {
        /* Count palindromes of length L */
        idx_t count;
        if (L == 1) {
            count = 8;                  /* 0–7 */
        } else {
            int half_len = (L + 1) / 2;   /* number of digits in the first half */
            for (int i = 1; i < half_len; ++i)
                pow8[i] = pow8[i - 1] * (idx_t)8;   /* 8^i */
            count = (idx_t)7 * pow8[half_len - 1];
        }

        if (cumulative + count >= n) {   /* the wanted term is within this block */
            idx_t r = n - cumulative - 1;          /* 0‑based index within this length */
            int half_len = (L + 1) / 2;
            uint8_t digits[32];                    /* enough for all practical lengths */

            if (L == 1) {
                digits[0] = (uint8_t)r;           /* 0 – 7 */
            } else {
                idx_t block = pow8[half_len - 1];
                idx_t first_digit = r / block + 1; /* 1 – 7, leading digit non‑zero */
                idx_t rest        = r % block;
                digits[0] = (uint8_t)first_digit;
                for (int pos = half_len - 1; pos >= 1; --pos) {
                    digits[pos] = (uint8_t)(rest % 8);
                    rest /= 8;
                }
            }

            /* Build the decimal value from the digit string (base‑10) */
            seq_t val = (seq_t)0;
            for (int i = 0; i < half_len; ++i)
                val = val * (seq_t)10 + (seq_t)digits[i];

            int mirror_start = (L % 2 == 0) ? half_len - 1 : half_len - 2;
            for (int i = mirror_start; i >= 0; --i)
                val = val * (seq_t)10 + (seq_t)digits[i];

            return val;
        }

        cumulative += count;
        ++L;
    }
}
