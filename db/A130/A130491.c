/*
 * A130491  a(n) = numerator of Sum_{k=1..n} k^mu(n+1-k), where mu(m) =
 *          A008683(m).
 * https://oeis.org/A130491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sum_{k=1..n} k^mu(n+1-k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where mu(m) is the Möbius function. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mobius.h"

/* Computes the nth term of A130491.  Index n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* Undefined for n = 0, but safeguard. */

    seq_t num = 0;                     /* Current numerator of the sum. */
    seq_t den = 1;                     /* Current denominator of the sum. */

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m   = n + 1 - k;         /* Argument for the Möbius function. */
        seq_t mu  = mobius(m);         /* 0, 1, or -1 (represented as max unsigned). */

        if (mu == 0)                  /* Term is 1. */
        {
            num += den;
        }
        else if (mu == 1)             /* Term is k. */
        {
            seq_t k_s = (seq_t)k;
            num += den * k_s;
        }
        else                          /* Term is 1/k (mu == -1). */
        {
            seq_t k_s  = (seq_t)k;
            seq_t tmp_num = num * k_s + den; /* New numerator. */
            seq_t tmp_den = den * k_s;       /* New denominator before reduction. */
            seq_t g = gcd(tmp_num, tmp_den); /* Reduce fraction. */
            num = tmp_num / g;
            den = tmp_den / g;
        }
    }

    return num;                        /* Return the numerator of the simplified sum. */
}
