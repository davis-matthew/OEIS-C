/*
 * A167877  Largest m<=n such that no carry occurs when adding m to n in
 *          ternary arithmetic.
 * https://oeis.org/A167877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = largest m ≤ n such that no carry occurs when adding m and
 *            n in ternary arithmetic (base‑3)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 separately. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Extract ternary digits of n (least significant first). */
    uint64_t tmp = (uint64_t)n;
    uint8_t digits[64];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    uint64_t m = 0;
    int less = 0; /* flag: have we already made m < n? */

    /* Process digits from most significant to least significant. */
    for (int i = len - 1; i >= 0; --i) {
        uint8_t ni = digits[i];
        uint8_t mi;

        if (!less) {
            if (ni == 2) {
                /* Must choose m_i = 0 to stay ≤ n; we become less now. */
                mi = 0;
                less = 1;
            } else {
                /* n_i is 0 or 1; setting m_i = n_i keeps m == n so far. */
                mi = ni;
            }
        } else {
            /* Already less; pick maximum allowed digit: 2 - n_i. */
            mi = 2 - ni;
        }

        m = m * 3 + mi;
    }

    return (seq_t)m;
}
