/*
 * A045243  Numbers whose base-5 representation contains exactly two 1's and
 *          three 3's.
 * https://oeis.org/A045243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045243
   Numbers whose base‑5 representation contains exactly two 1s
   and exactly three 3s.  The function returns the nth such
   number (1‑based index).  Computation is combinatorial:
   - Count how many numbers of each length satisfy the digit
     constraints, excluding leading zeros.
   - Locate the correct length for the given index.
   - Build the number digit by digit using the counts of the
     remaining required 1's and 3's.  */

#include <stdbool.h>

/* binomial coefficient C(n,k) – works for 0 ≤ k ≤ n */
static idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* construct 3^e (e ≥ 0) – e is small (≤ len) */
static idx_t pow3(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 3;
    return r;
}

/* Number of ways to fill `rem` positions with `o` ones and `t` threes
   (the other positions may be 0,2,4).  If impossible, returns 0. */
static idx_t count_completions(idx_t rem, idx_t o, idx_t t)
{
    if (o + t > rem) return 0;
    idx_t others = rem - o - t;
    idx_t ways = binom(rem, o);
    ways *= binom(rem - o, t);
    ways *= pow3(others);
    return ways;
}

seq_t a(idx_t n)
{
    /* Find the length L of the number that contains the nth term. */
    idx_t len = 5;                 /* minimal length: 2+3=5 */
    idx_t cumulative = 0;          /* numbers counted so far */

    while (true) {
        /* Count numbers of exactly `len` digits with the required
           pattern and a non‑zero most significant digit. */
        idx_t cnt_len = count_completions(len, 2, 3);
        /* Subtract the cases where the first digit is 0. */
        cnt_len -= count_completions(len - 1, 2, 3);

        if (cumulative + cnt_len >= n) break;
        cumulative += cnt_len;
        ++len;
    }

    /* Offset of the desired number within all numbers of this length. */
    idx_t offset = n - cumulative - 1;   /* zero‑based */

    seq_t result = 0;
    idx_t ones_needed   = 2;
    idx_t threes_needed = 3;

    for (idx_t pos = 0; pos < len; ++pos) {
        bool first_digit = (pos == 0);
        for (int d = first_digit ? 1 : 0; d <= 4; ++d) {
            idx_t o = ones_needed;
            idx_t t = threes_needed;

            if (d == 1) {
                if (o == 0) continue;
                --o;
            } else if (d == 3) {
                if (t == 0) continue;
                --t;
            }

            idx_t rem = len - pos - 1;
            idx_t cnt = count_completions(rem, o, t);

            if (offset < cnt) {
                result = result * (seq_t)5 + (seq_t)d;
                ones_needed   = o;
                threes_needed = t;
                break;
            } else {
                offset -= cnt;
            }
        }
    }
    return result;
}
