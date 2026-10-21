/*
 * A083466  Smallest nontrivial multiple of n ending in n. By nontrivial one
 *          means a(n) is not equal to n or concatenation of n with itself.
 * https://oeis.org/A083466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest nontrivial multiple of n ending in n.
   Let d = number of decimal digits of n and M = 10^d.
   We need the smallest integer k > 1 such that (k·n) mod M = n.
   This is equivalent to M | (k – 1)·n.
   Let g = gcd(n, M).  Then k – 1 must be a multiple of M/g.
   The smallest candidate is k = 1 + M/g.
   If g = 1 the candidate equals the concatenation n||n (k = M + 1),
   so we skip it and take the next multiple: k = 1 + 2·M.   */

#include <stdint.h>

/* Return 2^exp as seq_t.  exp ≥ 0. */
static seq_t pow2(int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= 2;
    return r;
}

/* Return 5^exp as seq_t.  exp ≥ 0. */
static seq_t pow5(int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= 5;
    return r;
}

/* Compute the number of decimal digits of n (n ≥ 1). */
static int digits_of(idx_t n)
{
    int d = 0;
    do {
        ++d;
        n /= 10;
    } while (n != 0);
    return d;
}

seq_t a(idx_t n)
{
    const idx_t orig_n = n;          /* keep original n for final multiplication */

    /* 1. number of digits */
    const int d = digits_of(n);

    /* 2. exponents of 2 and 5 in n */
    int c2 = 0, c5 = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0 && tmp != 0) {
        ++c2;
        tmp >>= 1;
    }
    tmp = n;
    while (tmp % 5 == 0 && tmp != 0) {
        ++c5;
        tmp /= 5;
    }

    /* 3. compute t = 10^d / gcd(n,10^d) */
    int e2 = (c2 < d) ? d - c2 : 0;   /* 2^(d - min(c2,d))  */
    int e5 = (c5 < d) ? d - c5 : 0;   /* 5^(d - min(c5,d))  */

    seq_t t = pow2(e2);
    t *= pow5(e5);                    /* t = (10^d) / gcd(n,10^d)   */

    seq_t result;
    if (c2 == 0 && c5 == 0) {        /* g == 1, need to skip n||n          */
        /* M = 10^d = 2^d * 5^d */
        seq_t M = pow2(d);
        M *= pow5(d);                 /* 10^d */

        seq_t twoM = M * 2;           /* 2 * M */
        seq_t k = 1 + twoM;           /* k = 1 + 2·M */
        result = k * orig_n;
    } else {
        seq_t k = 1 + t;              /* k = 1 + (10^d)/gcd(n,10^d) */
        result = k * orig_n;
    }
    return result;
}
