/*
 * A087999  a(n) is the LCM of palindromic divisors of n.
 * https://oeis.org/A087999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 50000, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 104 terms through n = 9800 (2026-11-01)
 * Helpers:   gcd, is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least common multiple of all palindromic divisors of n.
   a(n) = lcm{ d | d divides n and is_palindrome(d) }   */
/* Uses the utility functions gcd and is_palindrome. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_palindrome_base.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset starts at 1, but guard for 0 */
        return (seq_t)0;

    seq_t lcm = 1;              /* start with lcm of the empty set (1)  */
    uint64_t NN = (uint64_t)n;  /* use built-in type for loop */

    for (uint64_t d = 1; d <= NN / d; ++d) {
        if (NN % d != 0)
            continue;

        /* first divisor d */
        seq_t dseq = (seq_t)d;
        if (is_palindrome_base(dseq, 10)) {
            seq_t g = gcd(lcm, dseq);
            lcm = (lcm / g) * dseq;
        }

        /* complementary divisor NN / d, skip if equal */
        uint64_t o = NN / d;
        if (o != d) {
            seq_t oseq = (seq_t)o;
            if (is_palindrome_base(oseq, 10)) {
                seq_t g = gcd(lcm, oseq);
                lcm = (lcm / g) * oseq;
            }
        }
    }

    return lcm;
}
