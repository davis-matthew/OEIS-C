/*
 * A036103  A look-and-say sequence: each term summarizes the previous two
 *          terms.
 * https://oeis.org/A036103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(0) = 0, a(1) = 1; * a(n) (n ≥ 2) is the concatenation (in
 *            descending digit order) of the * pairs [count digit] formed from
 *            the total digit counts of a(n‑2) and a(n‑1). * That is, a(n)
 *            “says” how many times each digit appears in the two *
 *            predecessors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 17, u128 n <= 611,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void digit_freq(seq_t x, uint64_t f[10])
{
    for (int i = 0; i < 10; ++i) f[i] = 0;
    if (x == (seq_t)0) {
        f[0] = 1;
        return;
    }
    seq_t t = x;
    while (t > (seq_t)0) {
        uint8_t d = (uint8_t)(t % (seq_t)10);
        f[d]++;
        t /= (seq_t)10;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    uint64_t freq_pp[10] = {0}, freq_p[10] = {0}, freq_new[10];
    /* a(0) = 0  → one zero
     * a(1) = 1  → one one            */
    freq_pp[0] = 1;
    freq_p[1]  = 1;

    seq_t prevprev = 0, prev = 1, val = 0;

    for (idx_t k = 2; k <= n; ++k) {
        uint64_t freq[10];
        for (int d = 0; d < 10; ++d) freq[d] = freq_pp[d] + freq_p[d];

        /* build a(k) from freq */
        val = (seq_t)0;
        for (int d = 9; d >= 0; --d) {
            uint64_t cnt = freq[d];
            if (cnt == 0) continue;
            /* digits of cnt (least significant first) */
            uint8_t tmp[32];
            int len = 0;
            if (cnt == 0) {
                tmp[len++] = 0;
            } else {
                while (cnt > 0) {
                    tmp[len++] = (uint8_t)(cnt % 10);
                    cnt /= 10;
                }
            }
            /* append count digits in correct order */
            for (int j = len - 1; j >= 0; --j) {
                val = val * (seq_t)10 + (seq_t)tmp[j];
            }
            /* append the digit itself */
            val = val * (seq_t)10 + (seq_t)d;
        }

        /* compute frequencies of the new value for the next step */
        digit_freq(val, freq_new);

        /* shift states */
        prevprev = prev;
        prev = val;
        for (int d = 0; d < 10; ++d) {
            freq_pp[d] = freq_p[d];
            freq_p[d]  = freq_new[d];
        }
    }

    return prev;   /* a(n) */
}
