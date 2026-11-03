/*
 * A207669  Numbers that match polynomials irreducible (mod 3), with
 *          coefficients in {0,1,2}.
 * https://oeis.org/A207669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th element of OEIS sequence A207669
   by enumerating positive integers and testing whether their
   base‑3 coefficient polynomial (coefficients in {0,1,2})
   is irreducible over GF(3).  The function is memoised and
   can be called in any order of n.  It works for any seq_t/
   idx_t types supplied by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Polynomial long division over GF(3):
   divides(f, deg_f, g, deg_g) returns true iff g divides f.
   Both f and g are stored little‑endian (coeff[0] constant). */
static bool divides(const int *f, int deg_f, const int *g, int deg_g)
{
    if (deg_f < deg_g) return false;

    int tmp[32];
    for (int i = 0; i <= deg_f; ++i) tmp[i] = f[i];

    for (int i = deg_f; i >= deg_g; --i) {
        int coeff = tmp[i];
        if (coeff == 0) continue;
        int shift = i - deg_g;
        for (int j = 0; j <= deg_g; ++j) {
            int val = tmp[shift + j] - coeff * g[j];
            val %= 3;
            if (val < 0) val += 3;
            tmp[shift + j] = val;
        }
    }

    /* remainder must be zero */
    for (int i = 0; i < deg_g; ++i)
        if (tmp[i] != 0) return false;
    return true;
}

/* Convert integer n (>=1) into base‑3 coefficients.
   coeff[0] is the constant term.  On return *deg holds the
   degree (index of the most significant non‑zero digit).      */
static void to_coeff(idx_t n, int *coeff, int *deg)
{
    int i = 0;
    while (n > 0) {
        coeff[i] = n % 3;
        n /= 3;
        ++i;
    }
    *deg = i - 1;
}

/* 3^k for k = 0..20 (enough for the degrees that appear up
   to n ≈ 10000; higher values are never reached). */
static const int POW3[21] = {
    1,          3,          9,          27,         81,
    243,        729,        2187,        6561,       19683,
    59049,     177147,     531441,     1594323,    4782969,
    14348907,  43046721, 129140163, 387420489,1162261467
};

/* Test whether the polynomial represented by coeff[0..deg]
   (coefficients in {0,1,2}) is irreducible over GF(3).
   Leading coefficient may be 1 or 2; we normalise to monic. */
static bool is_irreducible(const int *coeff, int deg)
{
    if (deg == 0) return false;   /* constant polynomial */
    if (deg == 1) return true;    /* linear polynomials are irreducible */

    /* Normalise to monic polynomial */
    int lead = coeff[deg] % 3;
    if (lead == 0) return false;   /* impossible for non‑zero degree */
    int f[32];
    if (lead == 1) {
        for (int i = 0; i <= deg; ++i) f[i] = coeff[i] % 3;
    } else { /* lead == 2 => multiply by 2 (inverse of 2 in GF(3)) */
        for (int i = 0; i <= deg; ++i) {
            int v = coeff[i] * 2;
            f[i] = (v % 3);
        }
    }

    /* For degree ≥2, constant term 0 implies factor x */
    if (f[0] == 0) return false;

    /* Check for linear factors: evaluate at 0,1,2 */
    for (int r = 0; r <= 2; ++r) {
        int val = 0;
        for (int i = deg; i >= 0; --i)
            val = (val * r + f[i]) % 3;
        if (val == 0) return false;  /* root found */
    }

    /* Test all monic factors of degree 2 .. deg/2 */
    for (int k = 2; k <= deg / 2; ++k) {
        int total = POW3[k];
        for (int mask = 0; mask < total; ++mask) {
            int g[32];
            int tmp = mask;
            for (int j = 0; j < k; ++j) {
                g[j] = tmp % 3;
                tmp /= 3;
            }
            g[k] = 1;   /* monic coefficient */
            if (divides(f, deg, g, k)) return false; /* reducible */
        }
    }
    return true;  /* no factor found */
}

/* ---------- Memoisation and generation ---------- */

static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t cache_len = 0;      /* number of terms stored       */
static idx_t next_test_n = 1;     /* next integer to test */

/* Return the n-th element of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */

    while (cache_len < (size_t)n) {
        /* Convert current integer to base‑3 polynomial */
        int coeff[32];
        int deg;
        to_coeff(next_test_n, coeff, &deg);

        if (is_irreducible(coeff, deg)) {
            /* Resize cache and add term */
            seq_t *p = mem_resize(seq_t, seq_cache, (cache_len + 1));
            if (!p) abort();           /* allocation failure */
            seq_cache = p;
            seq_cache[cache_len] = (seq_t)next_test_n;
            ++cache_len;
        }
        ++next_test_n;
    }
    return seq_cache[n - 1];
}
