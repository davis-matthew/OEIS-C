/*
 * A043456  Numbers having four 0's in base 9.
 * https://oeis.org/A043456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having four 0's in base 9. */
/* This routine returns the n‑th such number (n ≥ 1). */

#include <stdbool.h>

/* Pre‑compute powers of 8 up to 8^255 (sufficient for all relevant indices). */
static seq_t pow8[256];
static bool pow8_initialized = false;

static void init_pow8(void) {
    if (pow8_initialized) return;
    pow8[0] = (seq_t)1;
    for (idx_t i = 1; i < 256; ++i)
        pow8[i] = pow8[i-1] * (seq_t)8;
    pow8_initialized = true;
}

/* Binomial coefficient C(n,k) using seq_t arithmetic. */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset is 1 */
    init_pow8();

    seq_t remaining = (seq_t)n;       /* position within the current length block */

    for (idx_t L = 5; ; ++L) {        /* minimum length is 5 (1 followed by four 0's) */
        seq_t count_len = binom(L-1, 4) * pow8[L-4];   /* numbers of length L */

        if (remaining <= count_len) {
            seq_t result = (seq_t)0;
            idx_t zeros_left = 4;

            for (idx_t pos = L; pos > 0; --pos) {
                idx_t rem_pos = pos - 1;    /* positions to the right of current */

                for (int d = (pos == L ? 1 : 0); d <= 8; ++d) {
                    seq_t ways;
                    if (d == 0) {
                        if (zeros_left == 0) continue;           /* cannot place more zeros */
                        ways = binom(rem_pos, zeros_left - 1) *
                               pow8[rem_pos - (zeros_left - 1)];
                    } else {
                        ways = binom(rem_pos, zeros_left) *
                               pow8[rem_pos - zeros_left];
                    }

                    if (remaining > ways) {
                        remaining -= ways;
                        continue;
                    }

                    /* choose this digit */
                    result = result * (seq_t)9 + (seq_t)d;
                    if (d == 0) zeros_left--;
                    break;
                }
            }
            return result;
        }
        remaining -= count_len;   /* skip this entire block */
    }
}
