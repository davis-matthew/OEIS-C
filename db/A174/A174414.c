/*
 * A174414  a(n) is the smallest natural number k such that the concatenation
 *          (n+k)||k is a prime number.
 * https://oeis.org/A174414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that the concatenation (n + k)||k is
 *            prime
 * Range:     u8 n <= 10, u16 n <= 1242, u32 n <= 1242, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, digit_len, gcd, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/gcd.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Converting index to seq_t once for arithmetic with seq_t values */
    const seq_t nseq = (seq_t)n;

    /* Search for the smallest k satisfying the condition */
    for (idx_t kk = 1; ; ++kk)
    {
        /* Only odd last digits can yield a prime (except 2) */
        const int last = kk % 10;
        if (!(last == 1 || last == 3 || last == 7 || last == 9))
            continue;

        /* k as seq_t for arithmetic */
        const seq_t k = (seq_t)kk;

        /* m = n + k */
        const seq_t m = nseq + k;

        /* If gcd(n, 10^d + 1) ≠ 1 we can skip this k (where d = digits(k)) */
        const int d = digit_len(k);
        const seq_t pow10d = pow10((idx_t)d);
        const seq_t mod = pow10d + (seq_t)1;
        if (gcd(nseq, mod) != (seq_t)1)
            continue;

        /* Concatenate (n + k) and k.  The helper expects idx_t arguments. */
        const seq_t candidate = concat_parts((idx_t)(n + kk), (idx_t)kk);

        /* Check primality */
        if (is_prime_big(candidate))
            return k;        /* k is already a seq_t, return it as the answer */
    }
}
