/*
 * A045203  Numbers whose base-5 representation contains no 0's and exactly
 *          one 4.
 * https://oeis.org/A045203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest number whose base‑5 digits contain no
 *            0's and exactly one digit 4
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* compute powers of 3 up to needed exponent */
    static seq_t pow3[65];
    static bool init = false;
    if (!init) {
        pow3[0] = (seq_t)1;
        for (int i = 1; i < 65; i++)
            pow3[i] = pow3[i-1] * (seq_t)3;
        init = true;
    }
    /* Find length k of the n-th term */
    seq_t n_t = (seq_t)n;
    seq_t total = (seq_t)0;     /* count of terms of lengths < k */
    seq_t cumulative = (seq_t)0;
    seq_t pow = (seq_t)1;       /* 3^(k-1) */
    int k = 0;
    for (int len = 1; ; len++) {
        k = len;
        seq_t count_len = (seq_t)len * pow;   /* len * 3^(len-1) */
        cumulative = total + count_len;
        if (n_t <= cumulative)
            break;
        total = cumulative;
        pow = pow * (seq_t)3;                 /* 3^len for next iteration */
    }
    /* Index within the length‑k block (0‑based) */
    seq_t m = n_t - total - 1;

    /* Build the base‑5 digits of the number */
    int digits[64];        /* maximum length we might need */
    bool used4 = false;
    seq_t remain = m;

    for (int pos = 0; pos < k; pos++) {
        int r = k - pos - 1;          /* remaining positions after this one */

        for (int d = 1; d <= 4; d++) {
            bool newUsed4 = used4 || (d == 4);
            bool need4 = !newUsed4;
            seq_t count;

            if (r == 0) {
                count = need4 ? (seq_t)0 : (seq_t)1;
            } else if (need4) {
                count = (seq_t)r * pow3[r - 1];   /* r * 3^(r-1) */
            } else {
                count = pow3[r];                  /* 3^r */
            }

            if (remain < count) {
                digits[pos] = d;
                used4 = newUsed4;
                break;
            } else {
                remain -= count;
            }
        }
    }

    /* Convert base‑5 digits to decimal */
    seq_t result = (seq_t)0;
    for (int i = 0; i < k; i++)
        result = result * (seq_t)5 + (seq_t)digits[i];

    return result;
}
