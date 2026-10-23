/*
 * A177979  Smallest number k such that A002313(n) divides k^2+1.
 * https://oeis.org/A177979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > 0 such that A002313(n) | (k^2 + 1) (A002313:
 *            primes ≡ 1 (mod 4) with 2 as the first element)
 * Range:     u8 n <= 62, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- prime tables (global, static) ---------- */
static uint64_t *all_primes     = NULL;   /* all primes found so far     */
static idx_t    all_count       = 0;
static idx_t    all_capacity    = 0;

static uint64_t *spec_primes    = NULL;   /* primes ≡ 1 (mod 4) (plus 2) */
static idx_t    spec_count      = 0;
static idx_t    spec_capacity   = 0;

/* next odd number to test for primality (after 3) */
static uint64_t next_candidate = 3;

/* ------------------------------------------------------------------ */
/* Add a new prime to the prime tables                                 */
static void add_prime(uint64_t p)
{
    if (all_count == all_capacity) {
        all_capacity = (all_capacity == 0) ? 8 : all_capacity * 2;
        all_primes = (uint64_t *)realloc(all_primes,
                                        all_capacity * sizeof(uint64_t));
    }
    all_primes[(size_t)all_count] = p;
    all_count++;

    if ((p == 2) || ((p & 3U) == 1U)) {          /* p % 4 == 1 or p == 2 */
        if (spec_count == spec_capacity) {
            spec_capacity = (spec_capacity == 0) ? 8 : spec_capacity * 2;
            spec_primes = (uint64_t *)realloc(spec_primes,
                                              spec_capacity * sizeof(uint64_t));
        }
        spec_primes[(size_t)spec_count] = p;
        spec_count++;
    }
}

/* ------------------------------------------------------------------ */
/* Generate primes until we have at least n special primes            */
static void ensure_special(idx_t n)
{
    if (spec_count >= n) return;

    /* Make sure 2 and 3 are present */
    if (all_count == 0) {
        add_prime(2);
        add_prime(3);
    }

    /* Basic trial‑division generation */
    while (spec_count < n) {
        uint64_t p = next_candidate;
        bool is_prime = true;
        for (idx_t i = 0; i < all_count && all_primes[i] * all_primes[i] <= p; ++i) {
            if (p % all_primes[i] == 0) {       /* composite */
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(p);
        next_candidate += 2;                    /* skip even numbers */
    }
}

/* ------------------------------------------------------------------ */
/* Modular exponentiation: base^exp mod mod (uint64_t version)       */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Tonelli–Shanks: square root of a modulo odd prime p                */
static uint64_t sqrt_mod_prime(uint64_t a, uint64_t p)
{
    if (p == 2) return a & 1;          /* trivial case */
    if (a == 0) return 0;
    /* Ensure a is a quadratic residue */
    if (mod_pow_uint64(a, (p - 1) / 2, p) != 1) return 0;

    /* Factor p-1 = q * 2^s with q odd */
    uint64_t q = p - 1;
    int s = 0;
    while ((q & 1ULL) == 0) { q >>= 1; ++s; }

    /* Find a quadratic non‑residue z */
    uint64_t z = 2;
    while (mod_pow_uint64(z, (p - 1) / 2, p) != p - 1) ++z;

    uint64_t c = mod_pow_uint64(z, q, p);
    uint64_t x = mod_pow_uint64(a, (q + 1) / 2, p);
    uint64_t t = mod_pow_uint64(a, q, p);
    int m = s;

    while (t != 1) {
        int i = 1;
        uint64_t t2 = (t * t) % p;
        while (t2 != 1) {
            t2 = (t2 * t2) % p;
            ++i;
        }
        uint64_t b = mod_pow_uint64(c, 1ULL << (m - i - 1), p);
        x = (x * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    return x;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */
    ensure_special(n);

    uint64_t p = spec_primes[(size_t)(n - 1)];
    uint64_t r = sqrt_mod_prime(p - 1, p);    /* r^2 ≡ −1 (mod p) */
    /* Choose the smaller positive residue */
    uint64_t k = r < (p - r) ? r : (p - r);
    return (seq_t)k;
}
