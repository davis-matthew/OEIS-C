/*
 * A214585  Numbers k such that gcd(k!!+1,k-1) > 1.
 * https://oeis.org/A214585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that gcd(k!!+1, k-1) > 1
 * Range:     u8 n <= 142, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool satisfies(idx_t k)
{
    if (k == 1) return true;          /* k-1 = 0, gcd(k!!+1,0) = k!!+1 > 1 */
    if ((k & 1) != 0) return true;   /* odd k → k!! odd ⇒ k!!+1 even, k-1 even ⇒ gcd ≥ 2 */

    /* k even.  Compute (k!!+1) modulo (k-1) and test gcd. */
    uint64_t mod = k - 1;
    uint64_t prod = 1;
    for (uint64_t i = 2; i <= k; i += 2) {
        prod = (prod * i) % mod;
    }
    uint64_t gmod = (prod + 1) % mod;

    /* Euclidean gcd on uint64_t */
    uint64_t a = gmod, b = mod;
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a > 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, give 0 */

    idx_t count = 0;
    idx_t k = 1;
    while (true) {
        if (satisfies(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
