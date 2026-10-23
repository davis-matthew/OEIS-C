/*
 * A103489  Multiplicative suborder of 3 (mod n) = sord(3, n).
 * https://oeis.org/A103489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimum positive integer e such that 3^e ≡ ±1 (mod n), or
 *            0 if no such e exists. For n=0,1 the value is set to 0
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 1)
        return 0;
    /* If 3 divides n, 3^k is always 0 mod 3 and never ±1 */
    if (n % 3 == 0)
        return 0;

    seq_t mod = (seq_t)n;
    seq_t val = ((seq_t)3) % mod;      /* 3^1 mod n */
    for (idx_t e = 1; e <= n; ++e)
    {
        if (val == (seq_t)1 || val == (mod - 1))
            return (seq_t)e;
        val = (val * ((seq_t)3)) % mod;   /* compute next power */
    }
    /* No exponent found up to n, so no solution */
    return 0;
}
