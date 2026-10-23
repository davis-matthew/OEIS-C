/*
 * A153728  Expansion of q^(-1/3) * (eta(q)^8 + 8 * eta(q^4)^8) in powers of
 *          q^2.
 * https://oeis.org/A153728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 92, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153728
 * Formula: a(n) = b(6*n + 1) where b is a multiplicative function defined by
 *   b(1) = 1,
 *   b(2^e) = b(3^e) = 0   (for e>0),
 *   if p ≡ 5 (mod 6) then for e even,
 *          b(p^e) = (-1)^(e/2) * p^(3e/2),
 *          for e odd  b(p^e) = 0,
 *   if p ≡ 1 (mod 6) then
 *          b(p)   = (x^2 – 3p) * x  where 4p = x^2 + 3y^2,
 *                         |x| < |y|  and  x ≡ 2 (mod 3),  choose such x;
 *          for e ≥ 2,
 *          b(p^e) = b(p) * b(p^(e-1)) – b(p^(e-2)) * p^3.
 *
 * The function below computes a(n) for any n ≥ 0 using the above definition.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Integer square root for uint64_t (floor) that works for C/C++        */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;  /* high^2 > any 64-bit value */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/* Fast binary exponentiation for seq_t                                 */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Global prime table (simple sieve limited to 70000)                   */
#define PRIME_LIMIT 70000
static uint32_t *prime_list   = NULL;
static size_t    prime_count  = 0;
static size_t    prime_cap    = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;

    unsigned char *iscomp = (unsigned char*)calloc(PRIME_LIMIT + 1, sizeof(unsigned char));
    if (!iscomp) return;                        /* allocation failed, abort */

    prime_cap  = 7000;                          /* approx number of primes < 70000 */
    prime_list = (uint32_t*)calloc(prime_cap, sizeof(uint32_t));
    if (!prime_list) { free(iscomp); return; }

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!iscomp[i]) {
            if (prime_count < prime_cap)
                prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    iscomp[j] = 1;
            }
        }
    }
    free(iscomp);
}

/* --------------------------------------------------------------------- */
/* Compute b(p) for a prime p ≡ 1 (mod 6) using the representation       */
/* 4p = x^2 + 3y^2 with |x| < |y| and x ≡ 2 (mod 3).                    */
static seq_t bp_of_prime(uint32_t p)
{
    uint64_t fourp = 4ULL * p;
    uint64_t maxy  = isqrt_u64(fourp / 3);

    for (uint64_t y = 1; y <= maxy; ++y) {
        uint64_t t = fourp - 3ULL * y * y;
        if (t == 0) continue;
        uint64_t x = isqrt_u64(t);
        if (x * x != t) continue;

        /* Try positive x */
        long long xc = (long long)x;
        long long mod = (xc % 3 + 3) % 3;
        if (mod == 2 && (uint64_t)xc < y) {
            seq_t xx = (seq_t)xc;
            return ((xx * xx) - (seq_t)(3 * p)) * xx;
        }
        /* Try negative x */
        long long xcneg = -xc;
        mod = (xcneg % 3 + 3) % 3;
        if (mod == 2 && (uint64_t)(-xcneg) < y) {
            seq_t xx = (seq_t)xcneg;
            return ((xx * xx) - (seq_t)(3 * p)) * xx;
        }
    }
    return 0;    /* Should never happen for valid primes */
}

/* --------------------------------------------------------------------- */
/* Compute b(p^e) for a prime p and exponent e                           */
static seq_t b_prime_power(uint32_t p, int e)
{
    if (e == 0) return 1;

    if (p == 2 || p == 3) return 0;          /* any positive exponent gives 0 */

    if (p % 6 == 5) {                        /* only even exponents matter */
        if (e & 1) return 0;
        int k = e / 2;
        seq_t val = pow_seq((seq_t)p, (idx_t)(3 * k));
        if (k & 1) val = -val;
        return val;
    }

    /* p ≡ 1 (mod 6) */
    seq_t bp = bp_of_prime(p);
    if (e == 1) return bp;

    seq_t p3 = pow_seq((seq_t)p, 3);
    seq_t b_prev2 = 1;
    seq_t b_prev1 = bp;
    seq_t b_curr = 0;
    for (int i = 2; i <= e; ++i) {
        b_curr = bp * b_prev1 - b_prev2 * p3;
        b_prev2 = b_prev1;
        b_prev1 = b_curr;
    }
    return b_curr;
}

/* --------------------------------------------------------------------- */
/* Multiplicative function b(n) for n ≥ 1                                 */
static seq_t b_value(uint64_t n)
{
    if (n == 1) return 1;

    init_primes();   /* ensure primes are ready */

    seq_t result = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t part = b_prime_power(p, e);
            result *= part;
            if (result == 0) return 0;
        }
    }

    if (tmp > 1) {                     /* remaining prime factor */
        seq_t part = b_prime_power((uint32_t)tmp, 1);
        result *= part;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
seq_t a(idx_t n)
{
    uint64_t k = 6ULL * n + 1;
    return b_value(k);
}
