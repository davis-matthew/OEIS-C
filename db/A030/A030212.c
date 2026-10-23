/*
 * A030212  Glaisher's chi_4(n).
 * https://oeis.org/A030212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 147, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS sequence A030212
   Glaisher's chi_4(n).

   The sequence is completely multiplicative with the following
   values for prime powers:

   • a(2^e)          = (-4)^e
   • if p ≡ 3 (mod 4):
         a(p^e)   = 0   if e odd,   = p^(2e) if e even
   • if p ≡ 1 (mod 4):
         a(p^e)   = a(p)·a(p^(e-1)) – p^4·a(p^(e-2))   (e≥2)
         a(p^1)   = 2·Re((x+i·y)^4) where p = x^2 + y^2
                     with even x and integer y.  (x,y) is obtained
                     by a simple search.

   The implementation factors the argument n by trial division
   (primes up to 10000 are generated once by a sieve) and combines
   the prime‑power contributions multiplicatively.
*/

#include <stdint.h>
#include <stddef.h>

/* forward declaration of the utility used in other files */
extern idx_t _smallest_prime_factor(uint64_t n);

/* ------------------------------------------------------------------ */
/*  Small integer square root for idx_t (uint64_t)                     */
static idx_t isqrt_idx(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/*  Find representation p = x^2 + y^2 with even x.  Return 1 on
    success, 0 otherwise (p is assumed prime ≡ 1 mod 4).           */
static int rep_x2_y2(idx_t p, idx_t *xp, idx_t *yp)
{
    idx_t limit = isqrt_idx(p);
    for (idx_t x = limit; x <= limit + 1; --x) {     /* start high, go down */
        idx_t y2 = p - x * x;
        if (y2 < 0) continue;
        idx_t y = isqrt_idx(y2);
        if (y * y == y2 && (x & 1) == 0) {
            *xp = x;
            *yp = y;
            return 1;
        }
        if (x == 0) break;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Compute a(p^e) for a prime p and exponent e (e ≥ 1).             */
static seq_t prime_power_value(idx_t p, idx_t e)
{
    if (e == 0)
        return 1;

    if (p == 2) {                     /* a(2^e) = (-4)^e */
        seq_t v = 1;
        for (idx_t i = 0; i < e; ++i)
            v *= (seq_t)(-4);
        return v;
    }

    if (p % 4 == 3) {                 /* p ≡ 3 (mod 4) */
        if (e & 1)
            return 0;                 /* odd exponent --> 0 */
        seq_t v = 1;
        for (idx_t i = 0; i < 2 * e; ++i)
            v *= (seq_t)p;
        return v;
    }

    /* p ≡ 1 (mod 4) */
    idx_t x, y;
    if (!rep_x2_y2(p, &x, &y))      /* should not happen for a prime ≡1 (mod4) */
        return 0;

    /* a(p) = 2*(x^4 - 6*x^2*y^2 + y^4) */
    seq_t xp = (seq_t)x;
    seq_t yp = (seq_t)y;
    seq_t ap = 2 * (xp * xp * xp * xp - 6 * xp * xp * yp * yp + yp * yp * yp * yp);

    if (e == 1)
        return ap;

    /* recurrence: a(p^e) = ap*a(p^(e-1)) - p^4*a(p^(e-2)) */
    seq_t p4 = (seq_t)p * p * p * p;
    seq_t prev2 = 1;      /* a(p^0) */
    seq_t prev1 = ap;     /* a(p^1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= e; ++i) {
        cur = ap * prev1 - p4 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/* ------------------------------------------------------------------ */
/*  Simple sieve of Eratosthenes up to 10000.  The result is stored
    in a static array that is reused on subsequent calls.               */
#define MAX_SIEVE 10000
static uint64_t primes[MAX_SIEVE];
static size_t prime_cnt = 0;
static int primes_ready = 0;

static void init_primes(void)
{
    if (primes_ready)
        return;

    size_t limit = MAX_SIEVE;
    uint8_t *is_composite = NULL;
    /* allocate memory for sieve bit array (uint8_t per position) */
    is_composite = (uint8_t*)malloc(limit + 1);
    if (!is_composite) return;          /* abort if allocation fails */

    for (size_t i = 0; i <= limit; ++i)
        is_composite[i] = 0;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (!is_composite[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_composite[q] = 1;
        }
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) primes[idx++] = i;

    prime_cnt = idx;
    primes_ready = 1;
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  It is safe for repeated, unordered calls. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, return 0 for n=0 is harmless */
        return 0;
    if (n == 1)
        return 1;        /* a(1) = 1 */

    init_primes();

    seq_t result = 1;
    idx_t temp = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            result *= prime_power_value(p, e);
        }
    }

    /* any remaining prime factor > sqrt(n) */
    if (temp > 1) {
        result *= prime_power_value(temp, 1);
    }
    return result;
}
