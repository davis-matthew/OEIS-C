/*
 * A098987  Numerators in series expansion of log(Product_{m>=0} (1+q^m)).
 * https://oeis.org/A098987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of ∑_{d|n} (-1)^(d+1)/d (equivalently, a(n) =
 *            (∑_{d|n} (-1)^(d+1) * n/d) / gcd(∑_{d|n} (-1)^(d+1) * n/d , n) )
 * Range:     u8 n <= 170, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean algorithm for seq_t (works for all integer and arbitrary‑precision types). */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sequence A098987 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) = 0 */

    seq_t nseq = (seq_t)n;
    seq_t pos = 0;         /* Σ_{d|n, d odd}  n/d */
    seq_t neg = 0;         /* Σ_{d|n, d even} n/d */

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t term = nseq / (seq_t)d;
            if ((d & 1) != 0)
                pos += term;
            else
                neg += term;

            /* counterpart n/d */
            idx_t e = n / d;
            if (e != d)
            {
                seq_t term2 = nseq / (seq_t)e;
                if ((e & 1) != 0)
                    pos += term2;
                else
                    neg += term2;
            }
        }
    }

    /* T = Σ_{d|n} (-1)^(d+1) * n/d = pos - neg (always non‑negative) */
    seq_t T = pos - neg;

    /* Reduce the fraction */
    seq_t g = seq_gcd(T, nseq);
    return T / g;
}
