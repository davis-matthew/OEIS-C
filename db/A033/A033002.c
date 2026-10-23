/*
 * A033002  Every run of digits of n in base 4 has length 2.
 * https://oeis.org/A033002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5 * V, where V is the n-th * base‑16 integer composed of
 *            digits 0,1,2,3 * with no leading zero and no two consecutive
 *            equal * digits. The generated numbers are listed in * increasing
 *            decimal order
 * Range:     u8 n <= 12, u16 n <= 120, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precompute powers of 3 up to a reasonable limit */
    static idx_t pow3[32];
    static int init = 0;
    if (!init) {
        pow3[0] = 1;
        for (int i = 1; i < 32; ++i) pow3[i] = pow3[i-1] * 3;
        init = 1;
    }

    idx_t remaining = n;          /* terms still to skip */

    /* find the length of the hexadecimal representation */
    for (int len = 1; ; ++len) {
        idx_t count_len = pow3[len];  /* 3^len possibilities for this length */
        if (remaining <= count_len) {
            /* Unrank the remaining-th sequence of this length */
            seq_t value = 0;
            int prev = -1;          /* previous character (none at start) */
            idx_t k = remaining;   /* position within block (1‑based) */

            for (int pos = 0; pos < len; ++pos) {
                for (int d = (pos==0 ? 1 : 0); d <= 3; ++d) {
                    if (pos > 0 && d == prev) continue;
                    idx_t suffix_cnt = pow3[len - pos - 1];
                    if (k > suffix_cnt) {
                        k -= suffix_cnt;
                    } else {
                        value = value * 16 + d;
                        prev = d;
                        break;
                    }
                }
            }
            return 5 * value;       /* final result */
        }
        remaining -= count_len;   /* skip whole block of this length */
    }
}
