/*
 * A082493  a(n) = n*ceiling(2^n/n) - 2^n.
 * https://oeis.org/A082493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = -(2^n) mod n
 * (equivalently  a(n) = (n - (2^n mod n)) mod n)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: function is not called with n==0 */

    /* compute 2^n mod n, using a 128‑bit intermediate to avoid overflow */
    idx_t rem = 1 % n;              /* initial value is 1 */
    for (idx_t i = 1; i <= n; ++i) {
        rem = ((unsigned __int128)rem * 2) % n;
    }

    /* a(n) = (n - rem) mod n; the result fits in seq_t */
    seq_t res = (seq_t)n - (seq_t)rem;
    res %= (seq_t)n;
    return res;
}
