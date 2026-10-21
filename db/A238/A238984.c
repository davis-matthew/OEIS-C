/*
 * A238984  a(n+1) is the product of the last three digits of the sequence up
 *          to a(n), start with a(0)=2.
 * https://oeis.org/A238984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 2; for n ≥ 0, a(n+1) is the product of the last three
 *            decimal digits of the concatenation of the sequence values
 *            a(0)…a(n). If fewer than three digits have appeared, the first
 *            digit is extended to the left. The recurrence can be computed
 *            iteratively while keeping only the last three digits...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)2;

    unsigned char last[3];
    int len = 0;

    /* start with a(0)=2 */
    last[len++] = 2;     /* len == 1 */
    seq_t val = 2;       /* current a(i) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod;
        if (len == 1) {
            prod = last[0] * last[0] * last[0];
        } else if (len == 2) {
            prod = last[1] * last[0] * last[0];
        } else { /* len >= 3 */
            prod = last[2] * last[1] * last[0];
        }
        val = prod;   /* a(i) */

        /* append the decimal digits of val to the running digit list */
        seq_t tmp = val;
        unsigned char buf[10];
        int dlen = 0;

        if (tmp == (seq_t)0) {
            buf[dlen++] = 0;
        } else {
            while (tmp > (seq_t)0) {
                buf[dlen++] = (unsigned char)(tmp % (seq_t)10);
                tmp /= (seq_t)10;
            }
        }

        /* write digits in correct order (most‑significant first) */
        for (int k = dlen - 1; k >= 0; --k) {
            unsigned char d = buf[k];
            if (len < 3) {
                last[len++] = d;
            } else {
                last[0] = last[1];
                last[1] = last[2];
                last[2] = d;
            }
        }
    }

    return val;
}
