/*
 * A028430  Clog sequence in base 9. Right to left concatenation of n,
 *          int(log_9(n)), int(log_9(int(log_9(n)))),... in base9.
 * https://oeis.org/A028430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation in base 9 of n, ⌊log₉ n⌋, ⌊log₉(⌊log₉ n⌋)⌋,
 *            … (right‑to‑left)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is defined; n ≥ 1 is guaranteed by caller. */
    seq_t result = 0;

    /* Step 1: extract base‑9 digits of n (least significant first). */
    idx_t temp = n;
    unsigned char n_rev_digits[32];
    int n_rev_len = 0;
    do {
        n_rev_digits[n_rev_len++] = (unsigned char)(temp % 9);
        temp /= 9;
    } while (temp > 0);

    /* Number of digits of n in base‑9. */
    idx_t log_n = (idx_t)(n_rev_len - 1);

    /* Step 2: build the sequence of logs: m₁ = ⌊log₉ n⌋, m₂ = ⌊log₉ m₁⌋, …, ending with 0. */
    idx_t logs[32];
    int logs_len = 0;
    idx_t m = log_n;
    while (1) {
        logs[logs_len++] = m;
        if (m == 0)
            break;
        /* compute ⌊log₉ m⌋; since m < 9⁶ for n ≤ 10 000, a simple loop suffices */
        idx_t t = m;
        idx_t lm = 0;
        while (t >= 9) {
            t /= 9;
            lm++;
        }
        m = lm;
    }

    /* Step 3: process logs from leftmost (last) to rightmost (first), feeding digits into result. */
    for (int i = logs_len - 1; i >= 0; --i) {
        idx_t val = logs[i];
        /* digits of val in base‑9, least significant first. */
        unsigned char val_rev[32];
        int val_rev_len = 0;
        if (val == 0) {
            val_rev_val:
            val_rev[val_rev_len++] = 0;
        } else {
            while (val > 0) {
                val_rev[val_rev_len++] = (unsigned char)(val % 9);
                val /= 9;
            }
        }
        /* append digits most significant first */
        for (int k = val_rev_len - 1; k >= 0; --k) {
            result = result * 9 + val_rev[k];
        }
    }

    /* Step 4: append the base‑9 digits of n (most significant first). */
    for (int k = n_rev_len - 1; k >= 0; --k) {
        result = result * 9 + n_rev_digits[k];
    }

    return result;
}
