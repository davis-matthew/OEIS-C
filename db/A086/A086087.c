/*
 * A086087  a(n) is the minimal m such that the group GL(m,3) has an element
 *          of order n.
 * https://oeis.org/A086087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): minimal m such that GL(m,3) has an element of order n.
   The algorithm factors the odd part of n, computes the multiplicative
   order of 3 modulo each prime power, and combines the results with
   the known structure of GL(m,3).  All operations are performed with
   integer arithmetic (no libraries required). */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* helper: gcd for idx_t (no external dependency) */
static inline idx_t my_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: fast modular exponentiation */
static inline idx_t powmod(idx_t base, idx_t exp, idx_t mod)
{
    idx_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) {
            res = (unsigned __int128)res * base % mod;
        }
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* helper: multiplicative order of 3 modulo p^e  (p≠3) */
static idx_t order_mod_prime_power(idx_t p, idx_t e)
{
    /* compute modulus p^e */
    idx_t mod = 1;
    for (idx_t i = 0; i < e; ++i) mod *= p;

    /* φ(p^e) = p^{e-1}*(p-1) */
    idx_t phi = mod / p * (p - 1);

    /* brute force search for the smallest d such that 3^d ≡ 1 (mod mod) */
    for (idx_t d = 1; d <= phi; ++d) {
        if (powmod(3, d, mod) == 1) return d;
    }
    /* should never happen for valid input */
    return phi;
}

/* helper: trial‑division factorisation into primes ≤ sqrt(n) */
static void factorize(idx_t n, int *primes, int *exps, int *cnt)
{
    *cnt = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            int c = 0;
            while (n % d == 0) { n /= d; ++c; }
            primes[(*cnt)] = (int)d;
            exps[(*cnt)]   = c;
            (*cnt)++;
        }
    }
    if (n > 1) {
        primes[(*cnt)] = (int)n;
        exps[(*cnt)]   = 1;
        (*cnt)++;
    }
}

/* ------------------------------------------------------------------ */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n<2 */

    /* separate the power of 3 from n */
    idx_t a3 = 0;
    idx_t tmp = n;
    while (tmp % 3 == 0) { tmp /= 3; ++a3; }
    idx_t r = tmp;                /* odd part, coprime to 3 */

    /* if n is a power of 3 */
    if (r == 1) {
        idx_t k = 1;
        for (idx_t i = 0; i < a3 - 1; ++i) k *= 3;
        k += 1;                   /* k = 3^(a3-1)+1  (k=2 when a3=1) */
        return (seq_t)k;
    }

    /* factor the odd part r */
    int primes[64], exps[64], pcnt;
    factorize(r, primes, exps, &pcnt);

    idx_t sum_d = 0;
    idx_t d_total = 1;
    idx_t dvec[64];
    for (int i = 0; i < pcnt; ++i) {
        idx_t di = order_mod_prime_power((idx_t)primes[i], (idx_t)exps[i]);
        dvec[i] = di;
        sum_d += di;
        idx_t g = my_gcd(d_total, di);
        d_total = d_total / g * di;       /* lcm */
    }

    /* 3‑part contribution */
    idx_t k = 1;
    if (a3 > 0) {
        for (idx_t i = 0; i < a3 - 1; ++i) k *= 3;
        k += 1;
    }

    /* if no 3‑part, the answer is simply d_total */
    if (a3 == 0) {
        return (seq_t)d_total;
    }

    /* combine with the 3‑part: various possibilities */
    idx_t best = sum_d + k;           /* separate blocks */
    idx_t opt2 = d_total * k;         /* combine with all prime parts */
    if (opt2 < best) best = opt2;

    for (int i = 0; i < pcnt; ++i) {
        idx_t opt = dvec[i] * k + (sum_d - dvec[i]);   /* combine with one part */
        if (opt < best) best = opt;
    }

    return (seq_t)best;
}
