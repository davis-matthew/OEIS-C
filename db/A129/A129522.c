/*
 * A129522  Expansion of unique weight 3 level 11 multiplicative cusp form in
 *          powers of q.
 * https://oeis.org/A129522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 68, i16 n <= 7106, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A129522:  Expansion of unique weight 3 level 11 multiplicative cusp form.
    a(1)=1.
    a(n) is multiplicative with the following rules:
      · a(11^e)   = (-11)^e.
      · If p≡2,6,7,8,10 (mod 11) then
            a(p^e) = 0                (e odd)
            a(p^e) = p^e              (e even)
      · If p≡1,3,4,5,9 (mod 11) let
            4p = y^2 + 11x^2   with y,x integers, y>0 having the smallest possible value.
            a(p)   = y^2 − 2p,
            a(p^e) = a(p)·a(p^(e−1)) − p^2·a(p^(e−2))   (e≥2)
    The sequence is defined for n≥1 only. No terms are required for n=0 or
    negative indices.  The function is fully multiplicative and works for
    arbitrary‑size seq_t’s or for signed integers of any supported width. */

#include <stdint.h>

/* ---------- Helper: integer square root (non‑negative) ---------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* ---------- Helper: fast exponentiation for seq_t ---------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* ---------- Helper: a(p) for primes p ≡ 1,3,4,5,9 (mod 11) ---------- */
static seq_t compute_a_prime(idx_t p)
{
    idx_t fourp = 4 * p;
    /* Search for the smallest y giving 4p = y² + 11x² */
    for (idx_t y = 0; y * y <= fourp; ++y) {
        idx_t diff = fourp - y * y;
        if (diff % 11 != 0)
            continue;
        idx_t x2 = diff / 11;
        idx_t xs = isqrt_u64(x2);
        if (xs * xs == x2) {                       /* valid representation found */
            seq_t y_sq = (seq_t)(y * y);
            seq_t two_p = (seq_t)(2 * p);
            return y_sq - two_p;                  /* y² – 2p, signed subtraction */
        }
    }
    /* Theory guarantees that a representation always exists for the primes
       of interest; returning 0 is a safe fallback. */
    return 0;
}

/* ---------- Helper: term for a prime power p^e ---------- */
static seq_t term_for_prime(idx_t p, idx_t e)
{
    if (p == 11)                                 /* a(11^e) = (-11)^e */
        return pow_seq((seq_t)-11, e);

    idx_t r = p % 11;
    if (r == 2 || r == 6 || r == 7 || r == 8 || r == 10) {
        /* a(p^e) = 0 if e odd, p^e if e even */
        if (e & 1)
            return 0;
        return pow_seq((seq_t)p, e);
    }

    /* r ∈ {1,3,4,5,9} */
    seq_t ap = compute_a_prime(p);                 /* a(p) */
    if (e == 1)
        return ap;

    /* Recurrence: a(p^e) = a(p)a(p^(e-1)) – p² a(p^(e-2))   */
    seq_t a0 = 1;                                   /* a(p^0) */
    seq_t a1 = ap;                                  /* a(p^1) */
    seq_t a2 = 0;
    for (idx_t k = 2; k <= e; ++k) {
        a2 = ap * a1 - (seq_t)p * p * a0;
        a0 = a1;
        a1 = a2;
    }
    return a2;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                          /* outside defined domain */

    idx_t m = n;
    seq_t result = 1;

    /* factor 2 separately */
    idx_t e = 0;
    while ((m & 1) == 0) {                                 /* count powers of 2 */
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t term = term_for_prime(2, e);
        if (term == 0)                /* then a(n)=0 */
            return 0;
        result *= term;
    }

    /* iterate over odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;
        e = 0;
        while (m % p == 0) {          /* count multiplicity of p */
            m /= p;
            ++e;
        }
        seq_t term = term_for_prime(p, e);
        if (term == 0)
            return 0;
        result *= term;
    }

    /* any remaining prime factor > sqrt(n) */
    if (m > 1) {
        seq_t term = term_for_prime(m, 1);
        if (term == 0)
            return 0;
        result *= term;
    }

    return result;
}
