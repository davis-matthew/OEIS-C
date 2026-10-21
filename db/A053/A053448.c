/*
 * A053448  Multiplicative order of 5 mod m, where gcd(m, 5) = 1.
 * https://oeis.org/A053448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 5 modulo floor((5*n-1)/4)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute the modulus m = floor((5*n-1)/4) */
    seq_t m = ((seq_t)5 * n - 1) / 4;

    /* For m == 1 the order is defined to be 1 (sequence a(1)=1) */
    if (m <= 1)
        return 1;

    /* Find the smallest k > 0 such that 5^k ≡ 1 (mod m) */
    seq_t x = 1;
    for (seq_t k = 1; ; ++k) {
        x = (x * 5) % m;      /* multiply by 5 and reduce modulo m */
        if (x == 1)
            return k;
    }
}
