/*
 * A088527  Define a Fibonacci-type sequence to be one of the form s(1) = s_1
 *          >= 1, s(2) = s_2 >= 1, s(n+2) = s(n+1) + s(n); then a(n) = maximal
 *          m such that n is the m-th term in some Fibonacci-type sequence.
 * https://oeis.org/A088527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute A088527: the maximum m such that the integer n can be
   the m‑th term of some Fibonacci‑type sequence
   s(1)=s1≥1, s(2)=s2≥1,
   s(k+2)=s(k+1)+s(k).
   The representation of n is n = s1·F_{m-2} + s2·F_{m-1} with F0=0, F1=1.
   For each m the pair (F_{m-2},F_{m-1}) are consecutive Fibonacci
   numbers, thus coprime.  We test solvability of
     s1·A + s2·B = n  with s1,s2≥1
   by using the general solution of the linear Diophantine equation.
*/

#include <stddef.h>
#include <stdbool.h>

#if defined(__SIZEOF_INT128__)
typedef __int128 signed_seq_t;
#else
typedef long long signed_seq_t;
#endif

/* ceil division for signed integers, denominator > 0 */
static signed_seq_t ceil_div_signed(signed_seq_t a, signed_seq_t b)
{
    if (b < 0) { a = -a; b = -b; }
    if (a >= 0) return (a + b - 1) / b;
    /* for negative a: truncate toward zero gives the ceil */
    return a / b;
}

/* floor division for signed integers, denominator > 0 */
static signed_seq_t floor_div_signed(signed_seq_t a, signed_seq_t b)
{
    if (b < 0) { a = -a; b = -b; }
    if (a >= 0) return a / b;
    /* for negative a: floor = -ceil(|a|/b) */
    return - ( ((-a) + b - 1) / b );
}

/* extended Euclid: find x,y such that a*x + b*y = gcd(a,b) (which is 1 here) */
static void extgcd_signed(signed_seq_t a, signed_seq_t b,
                          signed_seq_t *x, signed_seq_t *y)
{
    signed_seq_t old_r = a, r = b;
    signed_seq_t old_s = 1, s = 0;
    signed_seq_t old_t = 0, t = 1;
    while (r != 0) {
        signed_seq_t q = old_r / r;
        signed_seq_t tmp = r;
        r = old_r - q * r;
        old_r = tmp;
        tmp = s;
        s = old_s - q * s;
        old_s = tmp;
        tmp = t;
        t = old_t - q * t;
        old_t = tmp;
    }
    *x = old_s;
    *y = old_t;
}

/* Pre‑compute Fibonacci numbers (F0=0,F1=1,F2=1,…) up to a large bound */
static seq_t fib[200];
static bool fib_inited = false;

static void init_fib(void)
{
    if (fib_inited) return;
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    fib[2] = (seq_t)1;
    size_t i = 3;
    while (i < 200) {
        seq_t v = fib[i-1] + fib[i-2];
        fib[i] = v;
        if (v < fib[i-1]) break;      /* overflow guard */
        ++i;
    }
    fib_inited = true;
}

seq_t a(idx_t n)
{
    init_fib();

    /* Find the largest index m such that F_m <= n. For m=2, F2=1 ≤ n. */
    size_t max_m = 2;
    for (size_t i = 2; i < 200; ++i)
        if (fib[i] <= n) max_m = i;
        else break;

    /* Search m from high to low */
    for (size_t m = max_m; m >= 2; --m) {
        if (m == 2) return (seq_t)2;   /* always possible with s1=1, s2=n */

        seq_t A = fib[m-2];   /* F_{m-2} */
        seq_t B = fib[m-1];   /* F_{m-1} */

        /* A,B ≥ 1 for m ≥ 3 and are coprime. */
        signed_seq_t aS = (signed_seq_t)A;
        signed_seq_t bS = (signed_seq_t)B;
        signed_seq_t nS = (signed_seq_t)n;

        /* Find modular inverse of A modulo B */
        signed_seq_t x0, y0;
        extgcd_signed(aS, bS, &x0, &y0);   /* aS*x0 + bS*y0 = 1 */
        signed_seq_t invA = x0 % bS;
        if (invA < 0) invA += bS;

        /* particular solution a0 modulo B */
        signed_seq_t a0 = (nS * invA) % bS;
        if (a0 < 0) a0 += bS;

        /* Compute corresponding b0 */
        signed_seq_t b0 = (nS - a0 * aS) / bS;   /* exact division */

        /* Find integer t such that a = a0 + t*B ≥ 1 and
           b = b0 - t*A ≥ 1. */
        signed_seq_t t_min = ceil_div_signed(1 - a0, bS);
        signed_seq_t t_max = floor_div_signed(b0 - 1, aS);

        if (t_min <= t_max) {   /* representation exists */
            return (seq_t)m;
        }

        if (m == 3) break;   /* never possible to reach below m=2 */
    }
    return (seq_t)2;   /* fallback: m = 2 is always attainable */
}
