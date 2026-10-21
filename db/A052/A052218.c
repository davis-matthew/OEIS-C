/*
 * A052218  Numbers whose sum of digits is 4.
 * https://oeis.org/A052218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 70, u32 n <= 715, u64 n <= 8645,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose decimal digit sum equals 4, sorted in increasing order. */
/* Algorithm: combinatorial unranking – find the digit length that
   contains the n‑th term, then build the number digit by digit. */

#include <stdint.h>
#include <stddef.h>

/* Small binomial coefficient: C(n,k) for n,k <= 60 (fits in 64 bits). */
static uint64_t comb_small(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    idx_t remaining = n;           /* ordinal still to skip inside current length */
    idx_t len = 1;                 /* current digit length */

    /* Find the length that contains the n-th number. */
    while (1) {
        uint64_t total_len = 0;

        /* Count numbers of this length with digit sum 4. */
        for (uint8_t first = 1; first <= 4; ++first) {
            int s = 4 - first;               /* remaining sum after first digit */
            uint64_t cnt;
            if (s == 0) {
                /* sum already satisfied; all remaining digits must be 0 */
                cnt = 1;
            } else {
                uint64_t rem_pos = (uint64_t)len - 1;
                cnt = comb_small(rem_pos + (uint64_t)s - 1, (uint64_t)s);
            }
            total_len += cnt;
        }

        if (remaining > total_len) {
            remaining -= total_len;
            ++len;
        } else
            break;
    }

    /* Construct the number digit by digit. */
    seq_t result = 0;
    int sum_left = 4;

    for (idx_t pos = 1; pos <= len; ++pos) {
        uint8_t start_digit = (pos == 1) ? 1 : 0;
        uint8_t chosen = 0;

        for (uint8_t d = start_digit; d <= 9 && d <= sum_left; ++d) {
            int s_left = sum_left - d;
            uint64_t cnt;

            if (s_left == 0) {
                /* remaining digits must be all zeros – exactly one possibility */
                cnt = 1;
            } else {
                uint64_t rem_pos = len - pos;
                cnt = comb_small(rem_pos + (uint64_t)s_left - 1, (uint64_t)s_left);
            }

            if (remaining > cnt)
                remaining -= cnt;
            else {
                chosen = d;
                break;
            }
        }

        result = result * 10 + chosen;
        sum_left -= chosen;
    }

    return result;
}
