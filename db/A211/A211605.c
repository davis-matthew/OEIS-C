/*
 * A211605  a(n) is the nearest integer to x(n), where x(1) = 1, x(2) = 2,
 *          x(n) = operation (n-2 mod 4) applied to the pair (x(n-1), x(n-2)),
 *          where operation 1 is addition, operation 2 is multiplication,
 *          operation 3 is subtraction and operation 0 is division.
 * https://oeis.org/A211605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = nearest integer to the rational sequence defined by: *
 *            x(1) = 1, x(2) = 2. * For n ≥ 3: * let op = (n-2) mod 4, * op =
 *            0 → division, 1 → addition, 2 → multiplication, 3 → subtraction
 *            * x(n) = op(x(n-1), x(n-2)) * a(n) = round_half_up(x(n)) * * The
 *            implementation keeps a and b as rational n...
 * Range:     i8 n <= 171, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#if defined(__SIZEOF_INT128__)
  typedef __int128 int128_t;
  typedef unsigned __int128 uint128_t;
#else
  /* On targets without __int128, this needs to be replaced by a big-
   * integer library. The problem guarantees availability of such a
   * library for arbitrary‑precision builds, so this fallback is only for
   * illustrative purposes. */
  typedef int64_t int128_t;
  typedef uint64_t uint128_t;
#endif

/* Compute gcd of two signed 128-bit numbers. */
static int128_t _gcd128(int128_t a, int128_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int128_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Reduce a rational number represented by numerator and denominator. */
static void _reduce_rational(int128_t *num, int128_t *den)
{
    if (*den < 0) {
        *den = -(*den);
        *num = -(*num);
    }
    int128_t g = _gcd128(*num, *den);
    if (g != 0) {
        *num /= g;
        *den /= g;
    }
}

/* Addition of two rationals a/b and c/d. */
static void _add_rational(int128_t a_num, int128_t a_den,
                          int128_t b_num, int128_t b_den,
                          int128_t *res_num, int128_t *res_den)
{
    *res_num = a_num * b_den + b_num * a_den;
    *res_den = a_den * b_den;
    _reduce_rational(res_num, res_den);
}

/* Subtraction of two rationals a/b and c/d. */
static void _sub_rational(int128_t a_num, int128_t a_den,
                          int128_t b_num, int128_t b_den,
                          int128_t *res_num, int128_t *res_den)
{
    *res_num = a_num * b_den - b_num * a_den;
    *res_den = a_den * b_den;
    _reduce_rational(res_num, res_den);
}

/* Multiplication of two rationals a/b and c/d. */
static void _mul_rational(int128_t a_num, int128_t a_den,
                          int128_t b_num, int128_t b_den,
                          int128_t *res_num, int128_t *res_den)
{
    *res_num = a_num * b_num;
    *res_den = a_den * b_den;
    _reduce_rational(res_num, res_den);
}

/* Division of two rationals a/b divided by c/d. */
static void _div_rational(int128_t a_num, int128_t a_den,
                          int128_t b_num, int128_t b_den,
                          int128_t *res_num, int128_t *res_den)
{
    *res_num = a_num * b_den;
    *res_den = a_den * b_num;
    /* After division the denominator may be negative; normalize. */
    if (*res_den < 0) {
        *res_den = -(*res_den);
        *res_num = -(*res_num);
    }
    _reduce_rational(res_num, res_den);
}

/* Round a rational number to the nearest integer (half‑up). */
static seq_t _round128(int128_t num, int128_t den)
{
    if (num >= 0)
        return (seq_t)((num + den / 2) / den);
    else
        return (seq_t)((num - den / 2) / den);
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    int128_t p1_num = 2;          /* x(n‑1) */
    int128_t p1_den = 1;
    int128_t p2_num = 1;          /* x(n‑2) */
    int128_t p2_den = 1;
    int128_t cur_num, cur_den;

    for (idx_t i = 3; i <= n; ++i) {
        int op = (int)((i - 2) & 3);     /* 0: div, 1: add, 2: mul, 3: sub */
        switch (op) {
            case 0: _div_rational(p1_num, p1_den, p2_num, p2_den,
                                 &cur_num, &cur_den); break;
            case 1: _add_rational(p1_num, p1_den, p2_num, p2_den,
                                 &cur_num, &cur_den); break;
            case 2: _mul_rational(p1_num, p1_den, p2_num, p2_den,
                                 &cur_num, &cur_den); break;
            default: /* op==3 */ _sub_rational(p1_num, p1_den, p2_num, p2_den,
                                 &cur_num, &cur_den); break;
        }
        /* shift the window */
        p2_num = p1_num;
        p2_den = p1_den;
        p1_num = cur_num;
        p1_den = cur_den;
    }

    return _round128(p1_num, p1_den);
}
