/*
 * A140122  Negative of numerator of Sum_{k=1..n} (-1)^k / semiprime(k).
 * https://oeis.org/A140122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140122:  a(n) = -numerator of Σ_{k=1..n} (-1)^k / semiprime(k) */
/* The sum is always negative, so a(n) is the absolute value of the
 * reduced numerator of the fraction.  The function computes the sum
 * incrementally using only seq_t arithmetic; no external state is
 * required, so it works for any call order. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Test whether x is a semiprime (product of exactly two primes,
 * counting multiplicity). */
static bool
is_semiprime(uint64_t x)
{
    if (x < 4)
        return false;

    int cnt = 0;
    uint64_t tmp = x;

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        while (tmp % p == 0)
        {
            ++cnt;
            tmp /= p;
            if (cnt > 2)
                return false;
        }
    }

    if (tmp > 1)
        ++cnt;

    return cnt == 2;
}

/* Main sequence routine */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                /* defined only for n≥1, but harmless */

    seq_t sum_num = 0;            /* absolute value of numerator */
    seq_t sum_den = 1;            /* denominator, kept positive */
    bool sum_neg = false;         /* sign of the sum: true if negative */

    idx_t  found   = 0;             /* how many semiprimes seen */
    uint64_t cand = 4;               /* first possible semiprime */

    while (found < n)
    {
        if (is_semiprime(cand))
        {
            /* Candidate semiprime as seq_t */
            seq_t d = (seq_t)cand;
            bool term_neg = (found & 1U);   /* k = found+1, odd -> negative */

            if (sum_num == 0)          /* first term */
            {
                sum_num = 1;
                sum_den = d;
                sum_neg = term_neg;
            }
            else
            {
                seq_t g   = gcd(sum_den, d);
                seq_t left  = sum_num * (d / g);            /* numerator from existing sum */
                seq_t right = sum_den / g;                  /* numerator from new term (±1) */

                seq_t new_num;
                bool new_neg;

                if (sum_neg == term_neg)                /* same sign: add */
                {
                    new_num = left + right;
                    new_neg = sum_neg;
                }
                else                                       /* opposite signs: subtract */
                {
                    if (left >= right)
                    {
                        new_num = left - right;
                        new_neg = sum_neg;
                    }
                    else
                    {
                        new_num = right - left;
                        new_neg = term_neg;
                    }
                }

                if (new_num == 0)
                {
                    sum_num = 0;
                    sum_den = 1;
                    sum_neg = false;
                }
                else
                {
                    /* Reduce the fraction */
                    seq_t L   = (sum_den / g) * d;        /* lcm of denominators */
                    seq_t g2  = gcd(new_num, L);
                    sum_num   = new_num / g2;
                    sum_den   = L / g2;
                    sum_neg   = new_neg;
                }
            }

            ++found;
        }

        ++cand;
    }

    /* The sum is negative; return its absolute numerator. */
    return sum_num;
}
