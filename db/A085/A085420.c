/*
 * A085420  For each n, let p(n,b) be the smallest prime in the arithmetic
 *          progression k*n+b, with k > 0. Then a(n) = max(p(n,b)) with 0 < b
 *          < n and gcd(b,n) = 1.
 * https://oeis.org/A085420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 970, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085420
   Formula: a(n) = max { p(n,b) | 0 < b < n, gcd(b,n)=1 },
   where p(n,b) is the smallest prime in the arithmetic progression
   k*n + b with k > 0.
   The implementation enumerates each admissible residue class
   and searches for its first prime by incremental testing. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/* gcd for uint64_t ------------------------------------------------*/
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*------------------------------------------------------------------*/
/* modular multiplication using 128‑bit intermediary (% bug fest)  */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__int128)a * b) % mod);
}

/*------------------------------------------------------------------*/
/* modular exponentiation ------------------------------------------*/
static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e != 0) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/*------------------------------------------------------------------*/
/* deterministic Miller–Rabin primality test for 64‑bit integers   */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* small primes for trial division */
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for deterministic test up to 2^64 */
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/*------------------------------------------------------------------*/
/* main function computing a(n) -----------------------------------*/
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;           /* work in 64‑bit for speed */

    if (nn == 0) return (seq_t)0;        /* undefined but safe */
    if (nn == 1) return (seq_t)2;        /* a(1)=2 by definition */

    uint64_t best = 0;

    for (uint64_t b = 1; b < nn; ++b) {
        if (gcd_u64(b, nn) != 1) continue;   /* skip non‑coprime residues */

        /* find smallest prime congruent to b modulo nn */
        uint64_t cand = nn + b;              /* first candidate (k=1) */
        while (!is_prime_u64(cand)) cand += nn;
        if (cand > best) best = cand;
    }
    return (seq_t)best;
}
