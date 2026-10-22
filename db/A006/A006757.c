/*
 * A006757  The generalized Conway-Guy sequence w^{3}.
 * https://oeis.org/A006757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n+1) = 2*a(n) - a(n - r), r = round(sqrt(2*(n-3))), for n >=
 *            9, with a(0..9) = 0, 8, 10, 11, 12, 16, 32, 54, 97, 183
 *            (Lunnon's generalized Conway-Guy recurrence, shifted)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static const int init_vals[10] = {0, 8, 10, 11, 12, 16, 32, 54, 97, 183};

static seq_t *memo = 0;
static idx_t memo_len = 0;

static idx_t round_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    /* round(sqrt(x)) = r+1 iff x >= (r+1/2)^2 iff x > r^2 + r */
    if (x > r * r + r) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n >= memo_len) {
        idx_t nl = n + 1, i;
        if (nl < 2 * memo_len) nl = 2 * memo_len;
        memo = grow_zero(memo, memo_len, nl);
        for (i = memo_len; i < nl; i++) {
            if (i < 10) {
                memo[i] = (seq_t)init_vals[i];
            } else {
                idx_t m = i - 1;
                idx_t r = round_sqrt(2 * (m - 3));
                memo[i] = memo[m] + memo[m] - memo[m - r];
            }
        }
        memo_len = nl;
    }
    return memo[n];
}
