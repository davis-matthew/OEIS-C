/*
 * A058950  Coefficients of monic primitive irreducible polynomials over GF(5)
 *          listed in lexicographic order.
 * https://oeis.org/A058950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   computed via enumeration of monic primitive irreducible
 *            polynomials over GF(5) in lexicographic order
 * Range:     u8 n <= 6, u16 n <= 74, u32 n <= 354, u64 n <= 354,
 *            u128 n <= 354, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEG 10           /* sufficient for n up to 10000 */
static const int POW5[MAX_DEG+1] = {
    1,          /* 5^0   */
    5,          /* 5^1   */
   25,          /* 5^2   */
  125,          /* 5^3   */
  625,          /* 5^4   */
 3125,          /* 5^5   */
15625,          /* 5^6   */
78125,          /* 5^7   */
390625,         /* 5^8   */
1953125,        /* 5^9   */
9765625         /* 5^10  */
};

/* Inverses mod 5 */
static const uint8_t INV5[5] = {0,1,3,2,4};

/* ------------------------------------------------------------------ */
/* Polynomial arithmetic over GF(5)                                   */
/* ------------------------------------------------------------------ */

static void poly_mul_mod(const uint8_t *a, int da,
                         const uint8_t *b, int db,
                         const uint8_t *f, int df,
                         uint8_t *res)
{
    uint8_t prod[(MAX_DEG+1)*2];
    int prod_deg = da + db;
    memset(prod, 0, (prod_deg+1)*sizeof(uint8_t));

    for (int i=0; i<=da; ++i)
        for (int j=0; j<=db; ++j) {
            uint8_t mul = (a[i] * b[j]) % 5;
            int k = i + j;
            uint8_t tmp = prod[k] + mul;
            if (tmp >= 5) tmp -= 5;
            prod[k] = tmp;
        }

    int degR = prod_deg;
    while (degR >= 0 && prod[degR] == 0) --degR;

    while (degR >= df) {
        int shift = degR - df;
        uint8_t lead = prod[degR];
        if (lead != 0) {
            for (int k=0; k<=df; ++k) {
                uint8_t sub = (lead * f[k]) % 5;
                uint8_t val = prod[shift + k] + 5 - sub;
                if (val >= 5) val -= 5;
                prod[shift + k] = val;
            }
        }
        --degR;
        while (degR >= 0 && prod[degR] == 0) --degR;
    }
    for (int i=0; i<df; ++i) res[i] = prod[i];
    /* trailing coefficients beyond df are irrelevant */
}

static void poly_pow_mod(const uint8_t *f, int d,
                         uint64_t exp, uint8_t *res)
{
    uint8_t result[MAX_DEG+1] = {0};
    uint8_t base[MAX_DEG+1] = {0};
    uint8_t tmp[MAX_DEG+1];
    for (int i=0; i<d; ++i) result[i] = 0;
    result[0] = 1;            /* 1 */
    for (int i=0; i<d; ++i) base[i] = 0;
    if (d >= 1) base[1] = 1;  /* X */

    uint64_t e = exp;
    while (e) {
        if (e & 1ULL) {
            poly_mul_mod(result, d-1, base, d-1, f, d, tmp);
            memcpy(result, tmp, d * sizeof(uint8_t));
        }
        poly_mul_mod(base, d-1, base, d-1, f, d, tmp);
        memcpy(base, tmp, d * sizeof(uint8_t));
        e >>= 1;
    }
    memcpy(res, result, d * sizeof(uint8_t));
}

/* Euclidean algorithm for polynomials over GF(5) */
static int poly_gcd(const uint8_t *fa, int da,
                    const uint8_t *fb, int db)
{
    uint8_t A[MAX_DEG+1], B[MAX_DEG+1], R[MAX_DEG+1];
    memcpy(A, fa, (da+1)*sizeof(uint8_t));
    memcpy(B, fb, (db+1)*sizeof(uint8_t));

    int degA = da; while (degA>=0 && A[degA]==0) --degA;
    int degB = db; while (degB>=0 && B[degB]==0) --degB;

    while (degB >= 0) {
        /* R = A mod B */
        memcpy(R, A, (degA+1)*sizeof(uint8_t));
        int degR = degA;
        while (degR >= degB && degR >= 0) {
            uint8_t factor = (R[degR] * INV5[B[degB]]) % 5;
            int shift = degR - degB;
            if (factor != 0) {
                for (int i=0; i<=degB; ++i) {
                    uint8_t sub = (factor * B[i]) % 5;
                    uint8_t val = R[shift + i] + 5 - sub;
                    if (val >= 5) val -= 5;
                    R[shift + i] = val;
                }
            }
            while (degR >= 0 && R[degR] == 0) --degR;
        }
        /* set A = B, B = R */
        memcpy(A, B, (degB+1)*sizeof(uint8_t));
        degA = degB;
        memcpy(B, R, (degR+1)*sizeof(uint8_t));
        degB = degR;
    }
    return degA;    /* degree of GCD */
}

/* ------------------------------------------------------------------ */
/* Primitivity / irreducibility tests                                 */
/* ------------------------------------------------------------------ */

static bool is_irreducible(const uint8_t *f, int d)
{
    if (d == 1) return true;

    /* prime factors of d */
    int temp = d;
    int primes[8], pc = 0;
    for (int p=2; p*p<=temp; ++p) {
        if (temp % p == 0) {
            primes[pc++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[pc++] = temp;

    for (int i=0; i<pc; ++i) {
        int e = d / primes[i];
        uint8_t h[MAX_DEG+1];
        poly_pow_mod(f, d, POW5[e], h);           /* X^(5^e) mod f */

        /* diff = h - X */
        uint8_t diff[MAX_DEG+1] = {0};
        for (int j=0; j<d; ++j) diff[j] = h[j];
        diff[1] = (diff[1] + 4) % 5;              /* subtract 1 */

        if (poly_gcd(diff, d-1, f, d) > 0) return false;
    }

    /* final check: X^(5^d) mod f == X */
    uint8_t g[MAX_DEG+1];
    poly_pow_mod(f, d, POW5[d], g);
    for (int j=0; j<d; ++j) {
        uint8_t val = g[j];
        if (j == 1) {
            if (val != 1) return false;
        } else {
            if (val != 0) return false;
        }
    }
    return true;
}

static bool is_primitive(const uint8_t *f, int d)
{
    if (d == 1) {
        int a0 = f[0];
        int root = (5 - a0) % 5;                 /* root in GF(5) */
        return (root != 0) && ((root * root) % 5 != 1);
    }

    uint64_t m = (uint64_t)POW5[d] - 1ULL;       /* 5^d - 1 */
    uint64_t temp = m;
    uint64_t primes[20];
    int pc = 0;

    for (uint64_t p=2; p*p<=temp; ++p) {
        if (temp % p == 0) {
            primes[pc++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[pc++] = temp;

    for (int i=0; i<pc; ++i) {
        uint64_t exp = m / primes[i];
        uint8_t r[MAX_DEG+1];
        poly_pow_mod(f, d, exp, r);          /* X^exp mod f */
        /* check whether r == 1 */
        bool one = (r[0] == 1);
        for (int j=1; j<d; ++j) if (r[j] != 0) { one = false; break; }
        if (one) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence accessor                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t count = 0;
    /* enumerate degrees */
    for (int d=1; ; ++d) {
        if (d > MAX_DEG) {
            /* very unlikely for n <= 10000; if occurs, extend MAX_DEG */
            return (seq_t)0;
        }
        int total = POW5[d];

        for (int idx=0; idx<total; ++idx) {
            uint8_t coeff[MAX_DEG+1] = {0};
            int tmp = idx;
            for (int pos=0; pos<d; ++pos) {
                coeff[pos] = tmp % 5;
                tmp /= 5;
            }
            coeff[d] = 1;  /* monic */

            if (!is_irreducible(coeff, d)) continue;
            if (!is_primitive(coeff, d))   continue;

            ++count;
            if (count == n) {
                seq_t val = 0;
                for (int pos=d; pos>=0; --pos)
                    val = val * 10 + coeff[pos];
                return val;
            }
        }
    }
}
