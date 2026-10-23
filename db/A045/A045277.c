/*
 * A045277  Numbers whose base-5 representation contains exactly three 2's and
 *          three 3's.
 * https://oeis.org/A045277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045277: numbers whose base‑5 representation contains exactly three 2's and three 3's.
   The function computes a(n) by counting base‑5 strings with fixed digit counts
   and selecting the n‑th smallest value. */

#include <stdint.h>   /* for size_t, uint64_t, etc.  */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= base;
    return res;
}

static seq_t count_with(idx_t rem, idx_t r2, idx_t r3)
{
    if (r2 > rem || r3 > rem - r2) return 0;
    return binom(rem, r2) * binom(rem - r2, r3) * pow_int(3, rem - r2 - r3);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n < 1 is irrelevant */

    /* Find the length L of the number containing the nth element. */
    idx_t L = 6;
    seq_t cumulative = 0;

    for (;;) {
        seq_t total = binom(L, 3) * binom(L - 3, 3) * pow_int(3, L - 6);
        seq_t zero = 0;
        if (L > 6) zero = binom(L - 1, 3) * binom(L - 4, 3) * pow_int(3, L - 7);
        seq_t valid = total - zero;

        if (cumulative + valid >= n) break;
        cumulative += valid;
        ++L;
    }

    seq_t rem_n = n - cumulative;   /* 1‑based index within numbers of length L */

    /* Construct the digits from most significant to least significant. */
    uint8_t digits[64];             /* enough for all practical L */
    idx_t pos = 0;
    idx_t r2 = 3, r3 = 3;           /* remaining required 2's and 3's */

    while (pos < L) {
        idx_t rem_pos = L - pos - 1;

        for (uint8_t d = (pos == 0 ? 1 : 0); d <= 4; ++d) {
            if (d == 2 && r2 == 0) continue;
            if (d == 3 && r3 == 0) continue;

            idx_t nr2 = r2 - (d == 2 ? 1 : 0);
            idx_t nr3 = r3 - (d == 3 ? 1 : 0);
            seq_t cnt = count_with(rem_pos, nr2, nr3);

            if (rem_n > cnt) {
                rem_n -= cnt;
                continue;
            } else {
                digits[pos] = d;
                r2 = nr2;
                r3 = nr3;
                break;
            }
        }
        ++pos;
    }

    /* Convert the base‑5 digit array to decimal. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        val = val * 5 + digits[i];
    }
    return val;
}
