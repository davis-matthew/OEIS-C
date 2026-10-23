/*
 * A045255  Numbers whose base-5 representation contains exactly two 1's and
 *          no 4's.
 * https://oeis.org/A045255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth natural number whose base‑5 representation *
 *            contains exactly two digits equal to 1 and no digit equal to 4.
 *            * The function enumerates base‑5 strings of increasing length
 *            and * lexicographic order and selects the one at position n
 * Range:     u8 n <= 36, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* 3^exp  (exp is small for the tested range) */
static idx_t pow3(idx_t exp)
{
    idx_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 3;
    return res;
}

/* binomial coefficient C(n,k) for unsigned 64‑bit values */
static idx_t comb(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* symmetry */
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - i + 1) / i;
    return r;
}

/* Count of length‑len base‑5 strings using {0,1,2,3} that contain exactly
 * ones '1's. Leading zero is allowed; the caller handles it.
 */
static idx_t count_seq_rem(idx_t len, idx_t ones)
{
    if (ones > len) return 0;
    return comb(len, ones) * pow3(len - ones);
}

/* Total count of admissible numbers of exact base‑5 length L.
 * Leading digit cannot be 0, must be 1,2 or 3.
 */
static idx_t count_len(idx_t L)
{
    if (L < 2) return 0;
    idx_t total = 0;
    /* Leading digit = 1 */
    total += (L - 1) * pow3(L - 2);
    /* Leading digit = 2 or 3 */
    if (L >= 3)
        total += 2 * comb(L - 1, 2) * pow3(L - 3);
    return total;
}

seq_t a(idx_t n)
{
    /* Find the base‑5 length L that contains the nth number. */
    idx_t L = 2;
    idx_t cumulative = 0;
    for (;; ++L) {
        idx_t cnt = count_len(L);
        if (cumulative + cnt >= n)
            break;
        cumulative += cnt;
    }

    idx_t offset = n - cumulative;       /* 1‑based within this length */
    idx_t remain_ones = 2;
    seq_t value = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem_positions = L - pos - 1;

        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0)          /* no leading zero */
                continue;

            idx_t new_rem;
            if (d == 1) {
                if (remain_ones == 0)       /* too many 1's already used */
                    continue;
                new_rem = remain_ones - 1;
            } else {
                new_rem = remain_ones;
            }

            if (new_rem > rem_positions)    /* cannot place remaining 1's */
                continue;

            idx_t cnt = count_seq_rem(rem_positions, new_rem);
            if (offset > cnt) {
                offset -= cnt;
            } else {
                value = value * 5 + d;
                remain_ones = new_rem;
                break;                       /* move to next digit */
            }
        }
    }

    return value;
}
