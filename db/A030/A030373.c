/*
 * A030373  Write n in base 4 and juxtapose.
 * https://oeis.org/A030373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 4664, u16 n <= 4664, u32 n <= 4664, u64 n <= 4664,
 *            u128 n <= 4664, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A030373: The nth digit of the base‑4 Champernowne constant.
 * We write all positive integers in base‑4, concatenate their digits,
 * and a(n) is the nth digit (1‑based indexing).
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* `remaining` counts how many digits we still need to skip. */
    idx_t remaining = n;
    idx_t pow4 = 1;          /* 4^(d-1), where d = current digit length */
    int d = 1;               /* current digit length */

    while (1) {
        /* Count of d‑digit numbers in base‑4: 3 * 4^(d-1) */
        idx_t count_numbers = 3 * pow4;
        /* Total digits contributed by these numbers */
        idx_t digits_block = count_numbers * (idx_t)d;

        if (remaining <= digits_block) {
            /* The desired digit lies within this block. */
            idx_t number_index = (remaining - 1) / (idx_t)d;   /* 0‑based number offset */
            idx_t digit_index   = (remaining - 1) % (idx_t)d;   /* 0‑based position inside the number */

            /* The actual integer whose digits are being examined. */
            idx_t number = pow4 + number_index;                /* start value for d‑digit numbers is 4^(d-1) */

            /* Compute 4^(d-1-digit_index) to isolate the desired digit. */
            idx_t pow = 1;
            int exp = d - 1 - digit_index;
            for (int i = 0; i < exp; ++i) pow *= 4;

            /* Extract the digit: floor(number / 4^k) mod 4. */
            idx_t digit = (number / pow) % 4;
            return (seq_t)digit;
        }

        /* Skip this entire block and move to the next digit length. */
        remaining -= digits_block;
        ++d;
        pow4 *= 4;  /* Prepare pow4 for the next d (now 4^(d-1)). */
    }
}
