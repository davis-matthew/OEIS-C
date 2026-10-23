/*
 * A109200  Minimal value of k>0 such that n^5 + k^2 is a semiprime.
 * https://oeis.org/A109200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A109200:  minimal k>0 such that n^5 + k^2 is a semiprime.
    The routine uses Miller–Rabin primality testing and Pollard–Rho
    factorisation over 128‑bit unsigned integers to test semiprimality.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- helper functions for 128‑bit arithmetic ---------- */
static inline unsigned __int128 mod_mul(unsigned __int128 a, unsigned __int128 b, unsigned __int128 mod)
{
    return (a * b) % mod;
}

static unsigned __int128 mod_pow(unsigned __int128 a, unsigned __int128 d, unsigned __int128 mod)
{
    unsigned __int128 result = 1;
    a %= mod;
    while (d) {
        if (d & 1) result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return result;
}

static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- Miller–Rabin primality test (probabilistic) ---------- */
static bool is_prime_128(unsigned __int128 n)
{
    if (n < 2) return false;
    static const unsigned long long bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    unsigned __int128 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]) && bases[i] < (unsigned long long)n; i++) {
        unsigned __int128 a = bases[i];
        unsigned __int128 x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; r++) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Pollard–Rho factorisation ---------- */
static unsigned __int128 pollard_rho(unsigned __int128 n)
{
    if ((n & 1) == 0) return 2;
    static unsigned long long seed = 123456789;
    unsigned __int128 c = seed % n;
    seed += 123456;                           /* vary c each call */
    unsigned __int128 x = seed % n;
    unsigned __int128 y = x;
    unsigned __int128 d = 1;
    while (d == 1) {
        x = (mod_mul(x, x, n) + c) % n;
        y = (mod_mul(y, y, n) + c) % n;
        y = (mod_mul(y, y, n) + c) % n;
        d = gcd128(x > y ? x - y : y - x, n);
    }
    if (d == n) return pollard_rho(n);        /* retry with different parameters */
    return d;
}

static unsigned __int128 find_factor(unsigned __int128 n)
{
    if (n % 2 == 0) return 2;
    return pollard_rho(n);
}

/* ---------- semiprime test ---------- */
static bool is_semiprime(unsigned __int128 n)
{
    if (n < 4) return false;      /* 1,2,3 are not semiprimes */
    if (is_prime_128(n)) return false;
    unsigned __int128 d = find_factor(n);
    if (d == n) return false;    /* failed to find non‑trivial factor */
    unsigned __int128 q = n / d;
    return is_prime_128(d) && is_prime_128(q);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* compute n^5 as 128‑bit integer */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 val = 1;
    for (int i = 0; i < 5; i++) val *= n128;

    /* search for smallest k>0 such that n^5 + k^2 is semiprime */
    for (unsigned long long k = 1; ; k++) {
        unsigned __int128 target = val + (unsigned __int128)k * (unsigned __int128)k;
        if (is_semiprime(target))
            return (seq_t)k;   /* k is small, fits in seq_t */
    }
}
