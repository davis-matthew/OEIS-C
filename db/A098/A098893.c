/*
 * A098893  Sum of number of prime-factors of all prefixes in decimal
 *          representation of n.
 * https://oeis.org/A098893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Omega(n) + a(floor(n/10)) for n ≥ 10; a(n) = Omega(n) for
 *            1 ≤ n ≤ 9 where Omega(n) is the total number of prime factors of
 *            n (counted with multiplicity). The function below computes a(n)
 *            by iterating over all decimal prefixes of n and summing their
 *            Omega values
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   countOmega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/countOmega.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t cur = n;
    while (1) {
        result += countOmega(cur);
        if (cur < 10) break;
        cur /= 10;          /* remove the least significant decimal digit */
    }
    return result;
}
