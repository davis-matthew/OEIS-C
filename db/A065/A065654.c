/*
 * A065654  Fixed points for A065652, a permutation of the natural numbers.
 * https://oeis.org/A065654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((A065653(n) + A065653(n+1) - 1)/2), where
 *            A065653(0)=0, A065653(1)=1, A065653(k) = A065653(k-2)^2 + 2 for
 *            k>1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 23 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A065653_a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_nm2 = 0, a_nm1 = 1, cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = a_nm2 * a_nm2 + 2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}

seq_t a(idx_t n)
{
    seq_t x = A065653_a(n);
    seq_t y = A065653_a(n + 1);
    seq_t r = x / 2 + y / 2;
    if (((x & 1) + (y & 1)) == 0) r -= 1;
    return r;
}
