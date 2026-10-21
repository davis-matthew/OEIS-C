/*
 * A058944  Coefficients of monic irreducible polynomials over GF(3) listed in
 *          lexicographic order.
 * https://oeis.org/A058944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 3502, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   eval_at, pow3_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the coefficients of monic irreducible polynomials over GF(3) 
   listed in lexicographic order.  The term a(n) is the decimal
   representation of the n‑th polynomial.  This implementation
   enumerates all monic polynomials, tests them for irreducibility
   by checking for a linear root and for divisibility by all
   monic quadratics (which is sufficient for any degree greater
   than one).  The count of irreducibles of a given degree is
   obtained from the standard formula. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/eval_at.h"
#include "../utils/pow3_u64.h"

#define MAX_DEG 30   /* sufficient for n up to 10,000 */

/* ----------------------------------------------------------- */
/*  Möbius function (returns 0 if n has a repeated prime factor) */
static int mobius(int n)
{
    int cnt = 0;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
                if (exp > 1) return 0;
            }
            cnt++;
        }
    }
    if (n > 1) cnt++;               /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* Number of monic irreducible polynomials of degree d over GF(3) */
static uint64_t count_irreducible_deg(int d)
{
    uint64_t sum = 0;
    for (int k = 1; k <= d; ++k) {
        if (d % k == 0) sum += mobius(k) * pow3_u64(d / k);
    }
    return sum / (uint64_t)d;
}

/* ----------------------------------------------------------- */
/* Polynomial division over GF(3).  Returns true iff g divides f.
   Both f and g are represented in ascending coefficient order:
   f[0] is the constant term, f[deg_f] is the leading coefficient. */
static bool divides(const uint8_t *f, int deg_f,
                    const uint8_t *g, int deg_g)
{
    uint8_t rem[MAX_DEG + 1];
    memcpy(rem, f, (deg_f + 1) * sizeof(uint8_t));

    for (int i = deg_f; i >= deg_g; --i) {
        int factor = rem[i];
        if (factor == 0) continue;
        int shift = i - deg_g;
        for (int j = 0; j <= deg_g; ++j) {
            int idx = shift + j;
            int sub = (factor * g[j]) % 3;           /* 0,1,2 */
            int val = rem[idx] - sub;
            if (val < 0) val += 3;
            rem[idx] = (uint8_t)val;
        }
        rem[i] = 0;
    }

    for (int j = 0; j < deg_g; ++j)
        if (rem[j] != 0) return false;
    return true;
}

/* ----------------------------------------------------------- */

/* ----------------------------------------------------------- */
/* Test for irreducibility over GF(3).  f is monic.          */
static bool is_irreducible_candidate(const uint8_t *f, int deg)
{
    if (deg == 1) return true;  /* all linear are irreducible */

    /* Linear root test */
    for (int a = 0; a < 3; ++a)
        if (eval_at(f, deg, a) == 0) return false;

    /* Check divisors of degree 2 (sufficient for any d>1) */
    static uint8_t g[MAX_DEG + 1];
    static int    digits[MAX_DEG];

    for (int k = 2; k <= deg / 2; ++k) {
        for (int i = 0; i < k; ++i) digits[i] = 0;

        while (1) {
            /* build monic degree-k polynomial g */
            g[k] = 1;
            for (int j = 0; j < k; ++j)
                g[k - j - 1] = (uint8_t)digits[j];

            if (divides(f, deg, g, k)) return false;

            /* increment digits array lexicographically */
            int pos = k - 1;
            while (pos >= 0) {
                digits[pos]++;
                if (digits[pos] < 3) break;
                digits[pos] = 0;
                pos--;
            }
            if (pos < 0) break;            /* all combinations done */
        }
    }
    return true;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the degree that contains the n-th polynomial */
    idx_t cumulative = 0;
    int   deg = 0;
    idx_t offset = 0;

    while (1) {
        deg++;
        uint64_t cnt = count_irreducible_deg(deg);
        if (n <= cumulative + (idx_t)cnt) {
            offset = n - cumulative - 1;   /* zero‑based inside degree */
            break;
        }
        cumulative += cnt;
    }

    /* Enumerate all monic polynomials of this degree in lexicographic order */
    static int    digits[MAX_DEG];
    static uint8_t f[MAX_DEG + 1];

    for (int i = 0; i < deg; ++i) digits[i] = 0;

    idx_t found = 0;
    while (1) {
        /* build polynomial array f[0..deg] ascending */
        f[deg] = 1;                 /* leading coefficient */
        for (int j = 0; j < deg; ++j)
            f[deg - j - 1] = (uint8_t)digits[j];

        /* compute the decimal representation */
        seq_t val = 1;
        for (int j = 0; j < deg; ++j)
            val = val * 10 + (seq_t)digits[j];

        if (is_irreducible_candidate(f, deg)) {
            if (found == offset) return val;
            ++found;
        }

        /* increment digit array lexicographically (most significant changes slowest) */
        int pos = deg - 1;
        while (pos >= 0) {
            digits[pos]++;
            if (digits[pos] < 3) break;
            digits[pos] = 0;
            pos--;
        }
        if (pos < 0) break;                  /* all combinations exhausted */
    }

    /* Should never happen for valid n */
    return (seq_t)0;
}
