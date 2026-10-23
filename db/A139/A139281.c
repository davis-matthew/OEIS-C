/*
 * A139281  If all digits are the same mod 3, stop; otherwise write down the
 *          number formed by the 1 mod 3 digits and the number formed by the 2
 *          mod 3 digits and the number formed by the 3 mod 3 digits and
 *          multiply them; repeat.
 * https://oeis.org/A139281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is obtained by repeatedly replacing a number with the
 *            product of the numbers formed by its decimal digits that are ≡1
 *            (mod 3), ≡2 (mod 3), and ≡0 (mod 3) (the 0‐class). Digits are
 *            concatenated in the original left‑to‑right order within each
 *            class. The process stops when all digits belong to...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* A buffer large enough for the digit extraction routine. */
    static const int MAX_DIGITS = 2048;
    uint8_t stack[MAX_DIGITS];
    int len;

    while (1) {
        /* Extract decimal digits of x into stack (least‑significant
           digit first). */
        if (x == (seq_t)0) {
            len   = 1;
            stack[0] = 0;
        } else {
            seq_t tmp = x;
            len = 0;
            while (tmp != (seq_t)0 && len < MAX_DIGITS) {
                stack[len++] = (uint8_t)(tmp % (seq_t)10);
                tmp /= (seq_t)10;
            }
            /* The test data never exceeds the buffer size. */
        }

        /* Numbers for the three residue classes. */
        seq_t nums[3] = {0, 0, 0};
        bool seen[3]  = {false, false, false};

        for (int i = len - 1; i >= 0; --i) {       /* left‑to‑right */
            uint8_t d = stack[i];
            int r = d % 3;                        /* residue class */

            if (!seen[r]) {
                nums[r] = d;                      /* first digit */
                seen[r] = true;
            } else {
                nums[r] = nums[r] * 10 + d;       /* concatenate */
            }
        }

        int groups = 0;
        seq_t prod = 1;
        for (int r = 0; r < 3; ++r) {
            if (seen[r]) {
                ++groups;
                prod *= nums[r];
            }
        }

        if (groups == 1)                     /* all digits same mod 3 */
            return x;                        /* fixed point reached */

        x = prod;                            /* proceed to next step */
    }
}
