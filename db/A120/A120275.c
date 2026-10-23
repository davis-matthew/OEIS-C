/*
 * A120275  Smallest prime factor of the odd Catalan number A038003(n).
 * https://oeis.org/A120275
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime factor of the odd Catalan number A038003(n). */
/* The function computes the binomial(2^(n+1)-2, 2^n-1)/2^n and returns its smallest
   odd prime divisor.  For n≥2 the answer is always a prime ≥3. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp) */
#include "../utils/is_prime.h"  /* bool is_prime(idx_t n) */

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, so a(2) is the first term; the function
       ignores n<2, returning 0. */
    if (n < 2)
        return 0;

    /* m = 2^n - 1  (the index of the Catalan number) */
    seq_t m = pow2(n);
    m -= 1;

    /* Search for the smallest odd prime factor.  For an odd prime p,
       p divides the binomial coefficient iff adding m+m in base p
       causes at least one carry (Kummer's theorem).  Since the Catalan
       number equals that binomial divided by 2^n, an odd prime dividing
       the binomial necessarily divides the Catalan. */
    for (uint64_t p = 3; ; p += 2)
    {
        if (!is_prime((idx_t)p))
            continue;

        seq_t t = m;
        uint64_t carries = 0;
        seq_t carry = 0;

        /* Compute carries in base p. */
        while (t != 0)
        {
            seq_t digit = t % (seq_t)p;
            t       = t / (seq_t)p;
            seq_t sum = digit * 2 + carry;
            if (sum >= p)
            {
                carry = 1;
                ++carries;
            }
            else
            {
                carry = 0;
            }
        }

        if (carries > 0)
            return (seq_t)p;
    }
}
