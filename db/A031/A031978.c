/*
 * A031978  Omit digits 0, 1, 8 from n.
 * https://oeis.org/A031978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer obtained by removing all
 *            decimal digits 0, 1, and 8 from the successive integers starting
 *            with 1. For example, a(1)=2, a(2)=3, a(3)=4, a(7)=9, a(8)=2,
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build system.  Do not #define
   them here. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms produced so far   */
    idx_t k = 1;              /* current integer to process        */

    for (;;) {
        idx_t t = k;
        unsigned char digits[32];   /* buffer for digits, enough for 32‑bit numbers */
        int len = 0;

        /* Extract and keep only digits different from 0, 1, 8. */
        while (t > 0) {
            unsigned d = t % 10;
            if (d != 0 && d != 1 && d != 8) {
                digits[len++] = (unsigned char)d;
            }
            t /= 10;
        }

        /* If nothing remained after omission, skip this integer. */
        if (len == 0) {
            ++k;
            continue;
        }

        /* Reconstruct the integer from the kept digits in the
           correct order (most significant first). */
        seq_t val = 0;
        for (int i = len - 1; i >= 0; --i) {
            val = val * 10 + (seq_t)digits[i];
        }

        ++count;
        if (count == n) {
            return val;
        }
        ++k;
    }
}
