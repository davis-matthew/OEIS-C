/*
 * A045178  Numbers whose base-5 representation contains exactly one 0 and two
 *          2's.
 * https://oeis.org/A045178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly one 0 and
 *            two 2’s
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t comb(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

static seq_t count_rem(idx_t len, idx_t zeros, idx_t twos)
{
    if (zeros > len) return 0;
    if (twos > len - zeros) return 0;
    idx_t others = len - zeros - twos;
    seq_t r = comb(len, zeros);
    r = r * comb(len - zeros, twos);
    /* multiply by 3^others */
    for (idx_t i = 0; i < others; ++i)
        r = r * (seq_t)3;
    return r;
}

static seq_t count_len(idx_t len)
{
    /* count numbers of exact base‑5 length len */
    seq_t t1 = count_rem(len - 1, 1, 2);          /* leading digit 1,3,4 */
    seq_t t2 = count_rem(len - 1, 1, 1);          /* leading digit 2 */
    return t1 * (seq_t)3 + t2;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* not used per spec */

    /* find the length of the n‑th number */
    idx_t len = 3;
    seq_t cumulative = 0;
    seq_t cnt = count_len(len);
    while (cumulative + cnt < n) {
        cumulative += cnt;
        ++len;
        cnt = count_len(len);
    }
    /* offset inside this length group (0‑based) */
    seq_t offset = (n - cumulative) - 1;

    /* generate the number digit by digit */
    uint8_t digits[128];
    idx_t zeros_left = 1, twos_left = 2;

    for (idx_t pos = 0; pos < len; ++pos) {
        for (int cand = 0; cand <= 4; ++cand) {
            if (pos == 0 && cand == 0) continue;  /* no leading zero */
            idx_t z = zeros_left, t = twos_left;
            if (cand == 0) {
                if (z == 0) continue;
                --z;
            } else if (cand == 2) {
                if (t == 0) continue;
                --t;
            }
            idx_t rem_len = len - pos - 1;
            seq_t ways = count_rem(rem_len, z, t);
            if (offset < ways) {
                digits[pos] = (uint8_t)cand;
                zeros_left = z;
                twos_left = t;
                break;
            } else {
                offset -= ways;
            }
        }
    }

    /* convert digit array to decimal value */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        val = val * (seq_t)5 + (seq_t)digits[i];
    }
    return val;
}
