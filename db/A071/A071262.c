/*
 * A071262  Smallest composite number ending in n.
 * https://oeis.org/A071262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest composite number whose decimal representation ends with the
   decimal expansion of (n + 1).  That is, let d = n + 1 and let L be the number
   of decimal digits of d.  The desired a(n) is the minimal composite m such
   that m ≡ d (mod 10^L). */
#include <stdbool.h>

/* Simple deterministic primality test for values that comfortably fit in
   seq_t (used here because the expected search space is tiny). */
static bool is_prime(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    /* trial division up to sqrt(n) */
    for (idx_t i = 3; (seq_t)i * (seq_t)i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

static bool is_composite(seq_t n)
{
    return n > 1 && !is_prime(n);
}

seq_t a(idx_t n)
{
    /* d = n + 1, the required suffix */
    seq_t d = (seq_t)n + 1;

    /* determine the number of digits of d */
    idx_t len = 0;
    seq_t t   = d;
    if (t == 0)
        len = 1;
    else
        while (t)
        {
            t /= 10;
            ++len;
        }

    /* base = 10^len */
    seq_t base = 1;
    for (idx_t i = 0; i < len; ++i)
        base *= 10;

    /* search for the smallest composite m = k*base + d */
    for (idx_t k = 0;; ++k)
    {
        seq_t m = k * base + d;
        if (is_composite(m))
            return m;
    }
}
