/*
 * A098513  Number of points (x,y) on the elliptic curve y^2 = x^3 + x + 1
 *          (mod n), not including the point at infinity.
 * https://oeis.org/A098513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nice, nonn
 * Formula:   a(n) = Π over prime powers p^k dividing n of * a(p^k) where *
 *            a(2) = 2, a(2^k) = 2^(k-1) (k > 1) * a(p) = p + Σ_{x=0}^{p-1}
 *            Legendre(x^3+x+1 mod p) (odd prime) * a(p^k) = a(p) · p^(k-1)
 *            (odd prime, k ≥ 1) * * The sum of Legendre symbols counts the
 *            quadratic‐residue * contribution of the cubic pol...
 * Range:     u8 n <= 129, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Modulo exponentiation for seq_t types.  exp is small (≤5000). */
static seq_t modpow(seq_t base, unsigned exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (seq_t)((seq_t)result * b % mod);
        b = (seq_t)((seq_t)b * b % mod);
        exp >>= 1;
    }
    return result;
}

/* Cache for a(p) values, indexed by the prime itself. */
static seq_t ap_cache[10001] = {0};

/* Compute a(p) for an odd prime p.  The value is memoised. */
static seq_t calc_ap(unsigned p)
{
    if (ap_cache[p] != 0) return ap_cache[p];

    long long sum = 0;                 /* signed to allow negative values      */
    for (unsigned x = 0; x < p; ++x) {
        /* f(x) = x^3 + x + 1  (mod p) */
        seq_t xx = x;
        seq_t fx = (((xx * xx) % p) * xx % p + xx + 1) % p;

        if (fx == 0) continue;        /* Legendre(0) = 0, contributes nothing */

        seq_t leg = modpow(fx, (p - 1) / 2, p);
        if (leg == p - 1)            /* -1 residue ⇒ Legendre = -1 */
            sum -= 1;
        else                         /* Legendre = +1                 */
            sum += 1;
    }

    seq_t ap = (seq_t)((long long)p + sum);
    ap_cache[p] = ap;
    return ap;
}

/* Main function computing a(n).  No side effects beyond the cache. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;            /* by definition of the sequence */

    seq_t ans = 1;
    idx_t m = n;

    /* factor 2 separately */
    if ((m & 1) == 0) {
        unsigned e = 0;
        while ((m & 1) == 0) {
            ++e;
            m >>= 1;
        }
        seq_t val;
        if (e == 1)        /* a(2) = 2 */
            val = 2;
        else                /* a(2^k) = 2^(k-1)  for k > 1 */
            val = 1u << (e - 1);
        ans *= val;
    }

    /* factor odd primes */
    for (unsigned p = 3; (idx_t)p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t ap = calc_ap(p);
            seq_t powp = 1;
            for (unsigned i = 1; i < e; ++i)
                powp *= (seq_t)p;
            seq_t val = ap * powp;
            ans *= val;
        }
    }

    /* any remaining prime factor > sqrt(m) */
    if (m > 1) {
        seq_t ap = calc_ap((unsigned)m);
        seq_t val = ap;              /* exponent e = 1 */
        ans *= val;
    }

    return ans;
}
