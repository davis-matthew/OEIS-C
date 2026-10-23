/*
 * A031296  Position of n-th 9 in A007376.
 * https://oeis.org/A031296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   a(n) = the position of the n‑th digit 9 in the concatenation of
 *            the decimal representations of the positive integers (sequence
 *            A007376). The function simply scans the digits in the natural
 *            left‑to‑right order and counts occurrences of 9
 * Range:     u8 n <= 22, u16 n <= 5562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 safely */

    idx_t remaining = n;          /* number of 9’s still to find */
    idx_t pos       = 0;           /* current digit position (1‑based) */

    for (idx_t num = 1; ; ++num) {
        /* Extract the digits of num into a small buffer (reversed order). */
        int  digits[20];          /* 20 is enough for 64‑bit numbers */
        idx_t len = 0;
        idx_t t   = num;
        while (t) {
            digits[len++] = (int)(t % 10);
            t /= 10;
        }

        /* Scan from most significant to least significant digit. */
        for (idx_t i = 0; i < len; ++i) {
            ++pos;                      /* move to the next digit position */
            if (digits[len - 1 - i] == 9) { /* check the current digit */
                --remaining;
                if (remaining == 0) {
                    return (seq_t)pos;
                }
            }
        }
    }
}
