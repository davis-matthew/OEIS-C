/*
 * A141095  Number of unordered pairs of coprime nonprime numbers that sum to
 *          2n.
 * https://oeis.org/A141095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of unordered pairs of coprime nonprime numbers
 *            that sum to 2n
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

/* Count unordered coprime nonprime pairs that sum to 2n.
 * n >= 1.  For every a with 1 <= a <= n, let b = 2n - a.
 * The pair (a,b) is counted once if a <= b (guaranteed by a <= n),
 * both a and b are nonprime (i.e., not prime; 1 counts as nonprime),
 * and gcd(a,b) == 1.
 */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t twon = 2 * n;          /* 2n fits comfortably in idx_t */

    for (idx_t a = 1; a <= n; ++a)
    {
        idx_t b = twon - a;

        /* Skip if either member is prime.  is_prime returns false for 1,
         * so 1 is treated as nonprime automatically. */
        if (is_prime(a) || is_prime(b))
            continue;

        /* Count if coprime.  gcd expects seq_t arguments. */
        if (gcd((seq_t)a, (seq_t)b) == 1)
            ++count;
    }

    return count;
}
