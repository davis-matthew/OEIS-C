/*
 * A070669  Smallest m in range 2..n-1 such that m^4 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest m in [2..n-1] with m^4 ≡ 1 (mod n),
 *            or 1 if no such m exists.
 * The function performs a simple linear search using modular
 * exponentiation; it runs in O(n) time per call, which is
 * fast enough for n up to about 10,000.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 2) {
        return 1;
    }

    seq_t mod = (seq_t)n;          /* modulus as seq_t type */

    for (idx_t m = 2; m < n; ++m) {
        seq_t x = (seq_t)m;
        seq_t r = x % mod;          /* compute m^4 mod n */

        /* r = x^4 mod mod */
        r = (r * x) % mod;          /* x^2 */
        r = (r * x) % mod;          /* x^3 */
        r = (r * x) % mod;          /* x^4 */

        if (r == 1) {
            return m;
        }
    }

    return 1;
}
