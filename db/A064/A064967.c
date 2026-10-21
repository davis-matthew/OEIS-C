/*
 * A064967  100000000n+1, 100000000n+3, 100000000n+7, 100000000n+9 are all
 *          primes.
 * https://oeis.org/A064967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   modinv, mul_mod_u64, pow_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for numbers n such that
   100000000*n+1, 100000000*n+3, 100000000*n+7 and 100000000*n+9
   are all prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/modinv.h"
#include "../utils/mul_mod_u64.h"
#include "../utils/pow_mod_u64.h"

/* ----- constants ---------------------------------------------------------- */
#define PRIME_LIMIT 1000          /* primes used for quick filtering      */
#define B 100000000ULL           /* 10^8                                     */

/* ----- small‑prime filter ----------------------------------------------- */
static int prime_cnt = 0;
static unsigned primes[200];
static unsigned forb[200][4];      /* forbidden residues for each prime */

/* pre‑compute primes and the forbidden residues */
static bool initialized = false;
static void init_primes(void)
{
    bool is_comp[PRIME_LIMIT + 1] = {0};
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;

    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_comp[i]) continue;
        if (i == 2 || i == 5) continue;     /* 2 and 5 divide 10^8 and never divide the numbers */
        unsigned mul = (unsigned)(B % i);
        if (mul == 0) continue;                /* safety for divisibility by 2 or 5 */
        unsigned inv = modinv(mul, i);
        unsigned ks[4] = {1,3,7,9};
        for (int k = 0; k < 4; ++k) {
            unsigned rhs = ks[k] % i;
            unsigned val = (i - rhs) % i;        /* -rhs (mod i) */
            forb[prime_cnt][k] = (unsigned)((uint64_t)val * inv % i);
        }
        primes[prime_cnt++] = (unsigned)i;
    }
    initialized = true;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint16_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(*small); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(*bases); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!initialized) init_primes();

    uint64_t count = 0;
    uint64_t base = B;          /* base = B*m for current m */
    for (uint64_t m = 1; ; ++m) {
        /* quick small‑prime filter */
        bool bad = false;
        for (int i = 0; i < prime_cnt; ++i) {
            unsigned p = primes[i];
            unsigned r = (unsigned)(m % p);
            if (r == forb[i][0] || r == forb[i][1] ||
                r == forb[i][2] || r == forb[i][3]) {
                bad = true;
                break;
            }
        }
        if (!bad) {
            uint64_t v1 = base + 1;
            uint64_t v3 = base + 3;
            uint64_t v7 = base + 7;
            uint64_t v9 = base + 9;
            if (is_prime_u64(v1) && is_prime_u64(v3) &&
                is_prime_u64(v7) && is_prime_u64(v9)) {
                ++count;
                if (count == n)
                    return (seq_t)m;   /* found the n‑th term */
            }
        }
        base += B;
    }
}
