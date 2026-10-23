/*
 * A045236  Numbers whose base-5 representation contains exactly one 1 and no
 *          3's.
 * https://oeis.org/A045236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th positive integer whose base‑5
 * representation contains exactly one digit '1' and no digit '3'.
 * It first determines the base‑5 length of the n‑th number by
 * accumulating counts per length, then constructs the number
 * digit by digit, keeping track of the unique '1'.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    /* Precompute powers of 3 up to a reasonable limit */
    static seq_t pow3[70];
    static bool pow3_init = false;
    if (!pow3_init) {
        pow3[0] = 1;
        for (int i = 1; i < 70; ++i)
            pow3[i] = pow3[i - 1] * 3;
        pow3_init = true;
    }

    /* Find the length of base‑5 representation for a(n) */
    idx_t cumulative = 0;
    int len = 1;
    while (1) {
        seq_t count_len = (len == 1)
                          ? (seq_t)1
                          : pow3[len - 2] * ((seq_t)2 * len + 1);
        if (cumulative + (idx_t)count_len >= n)
            break;
        cumulative += (idx_t)count_len;
        ++len;
    }

    /* Position of a(n) within numbers of this length (1‑based) */
    idx_t k = n - cumulative;       /* k >= 1 */
    seq_t result = 0;
    bool one_used = false;          /* whether the digit '1' has been placed */

    /* Digits allowed in ascending order */
    const int digits[4] = {0, 1, 2, 4};

    for (int pos = 0; pos < len; ++pos) {
        int rem_pos = len - pos;         /* remaining positions including current */

        for (int di = 0; di < 4; ++di) {
            int dig = digits[di];

            /* Skip leading zero */
            if (pos == 0 && dig == 0)
                continue;
            /* Skip a second '1' */
            if (one_used && dig == 1)
                continue;

            bool new_used = one_used || (dig == 1);
            int rem_after = rem_pos - 1;
            seq_t completions;

            if (rem_after == 0) {
                completions = new_used ? (seq_t)1 : (seq_t)0;
            } else {
                if (new_used)
                    completions = pow3[rem_after];
                else
                    completions = (seq_t)rem_after * pow3[rem_after - 1];
            }

            if ((seq_t)k <= completions) {
                result = result * 5 + (seq_t)dig;
                one_used = new_used;
                break;
            } else {
                k -= (idx_t)completions;
            }
        }
    }

    return result;
}
