/*
 * A218085  Let S_5(x) denote the difference in counts of multiples of 5 in
 *          the interval [0,x), those with even digit sums in base 4 in one
 *          set, those with odd digit sums in base 4 in the other. Then a(n) =
 *          (-1)^s_4(n) *(S_5(n) -10*S_5(floor(n/16)) +5*S_5(floor(n/256))),
 *          where s_4(n) = A053737(n).
 * https://oeis.org/A218085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = (-1)^s_4(n) * (S_5(n) - 10*S_5(floor(n/16)) +
 *            5*S_5(floor(n/256)))
 * Range:     i8 n <= 2559, i16 n <= 2559, i32 n <= 2559, i64 n <= 2559,
 *            i128 n <= 2559, big any n
 * Validated: standard, pass, 97 terms through n = 2529 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where S_5(k) counts the multiples of 5 in [0,k) weighted by parity of base‑4 digit sums. */

#include <stdbool.h>

/* weighted count of multiples of 5 in [0, n) */
static seq_t S5(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t limit = (n - 1) / 5;      /* largest k with 5*k < n */
    seq_t sum = 0;
    for (idx_t k = 0; k <= limit; ++k)
    {
        idx_t m = 5 * k;
        int p = 0;
        idx_t t = m;
        while (t)
        {
            p ^= (int)(t & 1);
            t >>= 2;
        }
        sum += (p ? -1 : 1);
    }
    return sum;
}

/* parity of the sum of base‑4 digits of x (0 = even, 1 = odd) */
static int base4_parity(idx_t x)
{
    int p = 0;
    while (x)
    {
        p ^= (int)(x & 1);   /* low base‑4 digit parity is x & 1 */
        x >>= 2;            /* shift by two bits (divide by 4) */
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t s5   = S5(n);
    seq_t s5_16 = S5(n / 16);
    seq_t s5_256 = S5(n / 256);

    seq_t diff = s5 - 10 * s5_16 + 5 * s5_256;

    int p = base4_parity(n);
    seq_t sign = (p ? -1 : 1);

    return sign * diff;
}
