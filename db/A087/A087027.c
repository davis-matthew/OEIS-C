/*
 * A087027  a(n) = n*m where * is lunar multiplication and m is the ordinary
 *          sum n+1.
 * https://oeis.org/A087027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n ⊗ (n+1) where ⊗ is the lunar multiplication defined as
 *            follows: For each digit pair of the numbers a and b, take the
 *            minimum of the two digits. For every exponent i+j (where i and j
 *            are the positions of the digits from the right, starting at 0)
 *            select the maximum of all such minima. The...
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   decompose_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/decompose_digits.h"

seq_t a(idx_t n)
{
    /* Convert the inputs to seq_t and compute n+1 */
    seq_t a_val = (seq_t)n;
    seq_t b_val = a_val + (seq_t)1;

    /* Extract decimal digits of a and b (least significant first) */
    uint8_t dp[20];   /* maximum digits needed for n up to 10^5 */
    uint8_t dq[20];
    uint8_t len_p = 0;
    uint8_t len_q = 0;

    decompose_digits(a_val, dp, &len_p);
    decompose_digits(b_val, dq, &len_q);

    /* The product will have at most len_p + len_q digits */
    uint8_t res_digits[40] = {0};
    uint8_t max_len = len_p + len_q;

    /* Compute the digitwise lunar product */
    for (uint8_t i = 0; i < len_p; ++i) {
        for (uint8_t j = 0; j < len_q; ++j) {
            uint8_t m = dp[i] < dq[j] ? dp[i] : dq[j];
            uint8_t idx = i + j;
            if (m > res_digits[idx]) {
                res_digits[idx] = m;
            }
        }
    }

    /* Remove leading zeros (most significant side) */
    uint8_t ms = max_len - 1;
    while (ms > 0 && res_digits[ms] == 0) {
        ms--;
    }

    /* Convert the resulting digit array back to a seq_t */
    seq_t result = 0;
    for (int i = ms; i >= 0; --i) {
        result = result * ((seq_t)10) + ((seq_t)res_digits[i]);
    }

    return result;
}
