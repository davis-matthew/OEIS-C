/*
 * A064795  Home primes in base 2: primes reached when you start with n and
 *          (working in base 2) concatenate its prime factors (A048985);
 *          repeat until a prime is reached (or -1 if no prime is ever
 *          reached).
 * https://oeis.org/A064795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Home primes in base 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mod_mul.h"

/* --------------------------------------------------------------- */
/* small‑prime sieve up to 1 000 000 --------------------------------
   primes are needed for trial division in the factorisation stage
*/
#define SIEVE_LIMIT 1000000U
#define MAX_PRIMES  78498U          /* number of primes below 1e6 */

static uint32_t prime_list[MAX_PRIMES];
static size_t   prime_cnt   = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    primes_ready = true;
    static bool sieve[SIEVE_LIMIT + 1];
    memset(sieve, true, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = false;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = i;
}

/* --------------------------------------------------------------- */
/* 64‑bit deterministic Miller–Rabin --------------------------------
   works for all n < 341 550 071 728 321 (more than enough for this problem)
*/

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime checks */
    static const uint32_t small[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint32_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* miller‑rabin
       bases {2,3,5,7,11,13,17} are deterministic below 341 550 071 728 321
    */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/* prime factorisation of a 64‑bit integer ------------------------
   result is stored in the 'fac' array; its length in '*cnt'
*/
static void factor_uint64(uint64_t n, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        while (n % p == 0) {
            fac[(*cnt)++] = p;
            n /= p;
        }
    }
    if (n > 1) fac[(*cnt)++] = n;          /* remaining large prime */
}

/* --------------------------------------------------------------- */
/* bit length of a 64‑bit integer ---------------------------------
   returns number of bits required to represent x (at least 1)
*/
static unsigned bitlen_uint64(uint64_t x)
{
    unsigned len = 0;
    while (x) { len++; x >>= 1; }
    return len > 0 ? len : 1;
}

/* --------------------------------------------------------------- */
/* Convert a prime number to its binary representation written
   as a decimal string with digits 0/1, e.g. 5 (101b) -> 101 (int)
*/
static uint64_t binary_as_decimal(uint64_t prime)
{
    unsigned len = bitlen_uint64(prime);
    uint64_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        res = res * 10 + ((prime >> i) & 1ULL);
    }
    return res;
}

/* --------------------------------------------------------------- */
/* The requested sequence function --------------------------------
*/
extern seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* by convention */
    init_primes();

    uint64_t cur = (uint64_t)n;
    const int MAX_STEPS = 2000;    /* safety limit */
    for (int step = 0; step < MAX_STEPS; ++step) {

        /* check for primality */
        if (is_prime_uint64(cur)) {
            return (seq_t)binary_as_decimal(cur);
        }

        /* factorise current number */
        uint64_t factors[64];      /* enough for the expected range */
        size_t    fc = 0;
        factor_uint64(cur, factors, &fc);
        if (fc == 0) { /* should not happen, but fall back to -1 */
            return (seq_t)(-1);
        }

        /* sort factors increasingly (insertion sort, trivial size) */
        for (size_t i = 1; i < fc; ++i) {
            uint64_t key = factors[i];
            size_t j = i;
            while (j > 0 && factors[j-1] > key) {
                factors[j] = factors[j-1];
                --j;
            }
            factors[j] = key;
        }

        /* concatenate the binary representations of all factors */
        unsigned __int128 next = 0;
        for (size_t i = 0; i < fc; ++i) {
            uint64_t p = factors[i];
            unsigned len = bitlen_uint64(p);
            next = (next << len) | p;
            if (next > UINT64_MAX) break;      /* will overflow */
        }
        if (next > UINT64_MAX) {
            return (seq_t)(-1);     /* cannot continue safely */
        }
        cur = (uint64_t)next;
    }
    return (seq_t)(-1);             /* exceeded iteration limit */
}
