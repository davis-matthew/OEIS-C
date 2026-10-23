/*
 * A045057  Numbers whose base-4 representation contains exactly four 0's and
 *          no 2's.
 * https://oeis.org/A045057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   nth integer in base‑4 with exactly four zeros and no digit 2
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binomial coefficient C(n,k) using only integer arithmetic. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* 2^exp as seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * 2;
    return res;
}

/* Return the nth term of A045057 (base‑4 with exactly four 0’s and no 2’s). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    idx_t cumulative = 0;
    idx_t L = 5;                   /* minimal length (4 zeros + 1 non‑zero) */

    /* Find the length that contains the nth term. */
    while (1) {
        seq_t cnt = (binom(L, 4) - binom(L - 1, 3)) * pow2(L - 4);
        if (cumulative + (idx_t)cnt >= n) break;
        cumulative += (idx_t)cnt;
        ++L;
    }

    idx_t k = n - cumulative;      /* 1‑based rank within the block of length L */
    seq_t value = 0;
    idx_t zeros_left = 4;

    /* Build the number digit by digit in base‑4. */
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - 1 - pos;                 /* positions remaining after this one */
        int digits[3] = {0, 1, 3};
        int start = (pos == 0) ? 1 : 0;          /* first digit cannot be zero */

        for (int di = start; di < 3; ++di) {
            int d = digits[di];
            idx_t zeros_needed = zeros_left - (d == 0);
            if (zeros_needed > rem) continue;    /* impossible to satisfy zero count */

            seq_t cnt = binom(rem, zeros_needed) * pow2(rem - zeros_needed);

            if ((idx_t)cnt < k) {
                k -= (idx_t)cnt;                 /* skip this branch */
                continue;
            }

            /* Choose this digit and proceed to the next position. */
            value = value * 4 + d;
            if (d == 0) zeros_left -= 1;
            break;
        }
    }
    return value;
}
