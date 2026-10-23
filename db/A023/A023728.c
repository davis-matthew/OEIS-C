/*
 * A023728  Numbers with exactly 3 1's in base 5 expansion.
 * https://oeis.org/A023728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly 3 ones in their base‑5 expansion.
   Computes a(n) by counting how many numbers of each base‑5 length
   contain exactly three digits equal to 1, then constructing the
   nth such number digit by digit. */
#include <stdint.h>

/* Pre‑compute powers of 4 up to a generous limit. */
static seq_t pow4[201];
static int pow4_inited = 0;
static void init_pow4(void) {
    if (pow4_inited) return;
    pow4_inited = 1;
    pow4[0] = (seq_t)1;
    for (int i = 1; i < 201; ++i)
        pow4[i] = pow4[i - 1] * (seq_t)4;
}

/* Binomial coefficient C(n, k), for n ≤ 200. */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
        r = (r * (seq_t)(n - k + i)) / (seq_t)i;
    return r;
}

/* Count numbers of base‑5 length L that contain exactly three 1's
   and have a non‑zero most significant digit. */
static seq_t count_with_length(idx_t L) {
    if (L < 3) return (seq_t)0;

    /* Case 1: most significant digit is 1. */
    seq_t c1 = binom(L - 1, 2) * pow4[(int)(L - 3)] ;       /* choose 2 more 1's */

    /* Case 2: most significant digit is 2, 3 or 4. */
    seq_t c2 = 0;
    if (L >= 4) /* for L == 3, binom(L-1,3) == 0, so we skip */
        c2 = binom(L - 1, 3) * (seq_t)3 * pow4[(int)(L - 4)];  /* 3 choices for msd */

    return c1 + c2;
}

/* Count strings of length m that contain exactly k ones
   (other digits may be 0,2,3,4). */
static seq_t count_suffix(idx_t m, idx_t k) {
    if (k > m) return (seq_t)0;
    return binom(m, k) * pow4[(int)(m - k)];
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for offset handling */

    init_pow4();

    /* Find minimal length L such that cumulative count ≥ n. */
    idx_t L = 3;
    seq_t cumulative = (seq_t)0;
    seq_t countL;
    while (1) {
        countL = count_with_length(L);
        cumulative += countL;
        if (cumulative >= (seq_t)n) break;
        ++L;
    }

    /* Offset within numbers of length L (1‑based). */
    seq_t before = cumulative - countL;
    idx_t offset = (idx_t)((seq_t)n - before);   /* 1 ≤ offset ≤ countL */

    seq_t result = (seq_t)0;
    idx_t ones_left = 3;

    for (idx_t pos = (idx_t)0; pos < L; ++pos) {
        idx_t remaining = L - pos - 1;
        int d_start = (pos == 0) ? 1 : 0;   /* msd cannot be 0 */

        for (int d = d_start; d <= 4; ++d) {
            int sat_one = (d == 1) ? 1 : 0;
            if (ones_left < (idx_t)sat_one) continue;

            seq_t cnt = count_suffix(remaining, ones_left - sat_one);
            if (cnt < (seq_t)offset) {
                offset -= (idx_t)cnt;      /* skip this bunch */
            } else {
                result = result * (seq_t)5 + (seq_t)d;
                ones_left -= sat_one;
                break;                     /* move to next digit */
            }
        }
    }
    return result;
}
