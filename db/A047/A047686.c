/*
 * A047686  Denominators of coefficients in Taylor series for log(tan(x)/x).
 * https://oeis.org/A047686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of the coefficient of x^(2n) in
 *            log(tan(x)/x). The coefficient equals
 *            2^(2n)*(2^(2n-1)-1)*|B(2n)|/((2n)!*n) where B denotes the
 *            Bernoulli number. The denominator of this reduced fraction is
 *            returned
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

typedef struct {
    seq_t num;   /* absolute numerator (always non‑negative) */
    seq_t den;   /* positive denominator */
    int    sign; /* -1, 0, or 1 */
} Rat;

static Rat *B = NULL;      /* Bernoulli numbers up to the largest index requested */
static size_t B_max = 0;   /* highest Bernoulli index computed */

/* Reduce a rational number keeping its sign. */
static void rat_reduce(Rat *r)
{
    if (r->num == 0) {
        r->sign = 0;
        r->den   = 1;
        return;
    }
    seq_t g = gcd(r->num, r->den);
    r->num /= g;
    r->den /= g;
}

/* Compute Bernoulli numbers B0..maxIdx (inclusive). */
static void ensure_B(size_t maxIdx)
{
    if (maxIdx <= B_max) return;

    B = (Rat *)realloc(B, sizeof(Rat) * (maxIdx + 1));
    if (!B) exit(1);

    for (size_t i = B_max + 1; i <= maxIdx; ++i) {
        B[i].num  = 0;
        B[i].den  = 1;
        B[i].sign = 0;
    }

    /* B0 = 1 */
    if (B_max == 0 && B[0].sign == 0) {
        B[0].num  = 1;
        B[0].den  = 1;
        B[0].sign = 1;
    }

    /* Recurse: Bm = - (1/(m+1)) * Σ_{k=0}^{m-1} binom(m+1,k) * Bk   */
    for (size_t m = B_max + 1; m <= maxIdx; ++m) {
        Rat sum = {0, 1, 0};
        seq_t C  = 1;                       /* binom(m+1,0) */
        for (size_t k = 0; k < m; ++k) {
            seq_t term_num = B[k].num * C;
            seq_t term_den = B[k].den;
            Rat term = {term_num, term_den, B[k].sign};
            /* add term to sum */
            if (sum.sign == 0) {            /* sum is 0 */
                sum = term;
                seq_t g = gcd(sum.num, sum.den);
                sum.num /= g; sum.den /= g;
            } else if (sum.sign == term.sign) {  /* same sign */
                seq_t new_num = sum.num * term.den + sum.den * term_num;
                seq_t new_den = sum.den * term.den;
                sum.num = new_num; sum.den = new_den;
                seq_t g = gcd(sum.num, sum.den);
                sum.num /= g; sum.den /= g;
            } else { /* opposite signs */
                seq_t left  = sum.num * term.den;
                seq_t right = sum.den * term_num;
                if (left == right) {
                    sum.num = 0; sum.den = 1; sum.sign = 0;
                } else if (left > right) {
                    seq_t new_num = left - right;
                    seq_t new_den = sum.den * term.den;
                    sum.num = new_num; sum.den = new_den;
                    sum.sign = sum.sign;
                    seq_t g = gcd(sum.num, sum.den);
                    sum.num /= g; sum.den /= g;
                } else {
                    seq_t new_num = right - left;
                    seq_t new_den = sum.den * term.den;
                    sum.num = new_num; sum.den = new_den;
                    sum.sign = term.sign;
                    seq_t g = gcd(sum.num, sum.den);
                    sum.num /= g; sum.den /= g;
                }
            }
            /* update C = C * (m+1-k) / (k+1) */
            seq_t tmp = C * (m + 1 - k);
            C = tmp / (k + 1);
        }
        /* Bm = -sum / (m+1) */
        sum.sign = -sum.sign;
        seq_t new_den = sum.den * (m + 1);
        B[m].num  = sum.num;
        B[m].den  = new_den;
        B[m].sign = sum.sign;
        rat_reduce(&B[m]);            /* final reduction */
    }
    B_max = maxIdx;
}

/* Small helper for 128‑bit multiply when seq_t <= 64 bits. */
static seq_t mul128(seq_t a, seq_t b)
{
    return (seq_t)((__int128)a * b);
}

/* Main routine required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* a(0) = 1 */

    /* Compute Bernoulli number B_{2n}. */
    ensure_B((size_t)(2 * n));

    seq_t num = 1;               /* numerator of the fraction */
    seq_t den = 1;               /* denominator of the fraction */

    /* ---------- SMALL‑TYPE PATH (seq_t <= 64 bits) ---------- */
    if (sizeof(seq_t) <= 8) {
        /* multiply numerator by 2^(2n) */
        for (idx_t i = 0; i < 2 * n; ++i) {
            num = mul128(num, 2);
            seq_t g = gcd(num, den);
            if (g > 1) { num /= g; den /= g; }
        }
        /* compute 2^(2n-1)-1 */
        seq_t pow2_odd = 1;
        for (idx_t i = 0; i < 2 * n - 1; ++i) pow2_odd = mul128(pow2_odd, 2);
        seq_t term3 = pow2_odd - 1;
        num = mul128(num, term3);
        seq_t g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        /* multiply by |B_{2n}| numerator */
        seq_t Bnum = B[2 * n].num;
        num = mul128(num, Bnum);
        g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        /* denominator part: |B_{2n}| denominator */
        seq_t den_B = B[2 * n].den;
        den = mul128(den, den_B);
        g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        /* multiply denominator by (2n)! */
        for (idx_t i = 2; i <= 2 * n; ++i) {
            den = mul128(den, i);
            g = gcd(num, den);
            if (g > 1) { num /= g; den /= g; }
        }
        /* multiply denominator by n */
        den = mul128(den, n);
        g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        return den;
    }

    /* ---------- BIG‑INT PATH (seq_t is arbitrary precision) ---------- */
    {
        /* multiply numerator by 2^(2n) */
        for (idx_t i = 0; i < 2 * n; ++i) {
            num = num * (seq_t)2;
            seq_t g = gcd(num, den);
            if (g > 1) { num /= g; den /= g; }
        }
        /* compute 2^(2n-1)-1 using big‑int pow */
        seq_t pow2_odd = pow_uint(2, 2 * n - 1);
        seq_t term3 = pow2_odd - (seq_t)1;
        num = num * term3;
        seq_t g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        /* multiply by |B_{2n}| numerator */
        seq_t Bnum = B[2 * n].num;
        num = num * Bnum;
        g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        /* denominator part: |B_{2n}| denominator */
        seq_t den_B = B[2 * n].den;
        den = den * den_B;
        g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        /* multiply denominator by (2n)! */
        for (idx_t i = 2; i <= 2 * n; ++i) {
            den = den * i;
            g = gcd(num, den);
            if (g > 1) { num /= g; den /= g; }
        }
        /* multiply denominator by n */
        den = den * n;
        g = gcd(num, den);
        if (g > 1) { num /= g; den /= g; }
        return den;
    }
}
