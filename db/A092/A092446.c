/*
 * A092446  Numbers k such that if P = 10*k^2+1, then P, P+6, P+12 and P+18
 *          are all primes.
 * https://oeis.org/A092446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 42, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092446:
 * a(n) = k such that P = 10*k^2+1, P+6, P+12 and P+18 are all prime.
 * We generate terms by checking candidates in increasing k and
 * memoise them in a static cache.  A pre‑computed wheel of residues
 * modulo 510510 (product of primes 2,3,5,7,11,13,17) is used to skip
 * obvious composites quickly.  The wheel is constructed carefully to
 * allow the cases where the number equals one of the small primes. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static const uint32_t SMALL_PRIMES[] = {2, 3, 5, 7, 11, 13, 17};
static const size_t NUM_SMALL_PRIMES = sizeof SMALL_PRIMES / sizeof SMALL_PRIMES[0];

/* Product of the small primes: 2*3*5*7*11*13*17 = 510510. */
static const uint32_t WHEEL_MOD = 510510;

/* Residues modulo WHEEL_MOD that are not ruled out by small prime
 * divisibility tests.  1 = ok, 0 = reject. */
static uint8_t *good_residues = NULL;
static bool good_residues_ready = false;

/* Memoisation cache: stores found k values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;

/* Ensure the cache can hold at least 'need' entries. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Build the good_residues table.  For each residue r modulo WHEEL_MOD
 * we check that for all small primes p and offsets v∈{1,7,13,19}
 * the value 10*r^2+v is NOT divisible by p, except when that value
 * equals p itself (which is prime). */
static void build_good_residues(void)
{
    good_residues = (uint8_t *)malloc(WHEEL_MOD * sizeof(uint8_t));
    if (!good_residues)
        abort();

    for (uint32_t r = 0; r < WHEEL_MOD; ++r) {
        bool ok = true;
        uint64_t num_base = (uint64_t)10 * r * r;      /* 10*r^2  */
        for (size_t pi = 0; pi < NUM_SMALL_PRIMES && ok; ++pi) {
            uint32_t p = SMALL_PRIMES[pi];
            uint32_t r_mod = r % p;
            uint32_t sq_mod = (r_mod * r_mod) % p;
            uint32_t ten_mod = 10 % p;
            for (uint32_t v = 1; v <= 19; v += 6) {     /* 1,7,13,19 */
                uint32_t val = (ten_mod * sq_mod + v) % p;
                if (val == 0) {
                    /* The number 10*r^2+v is divisible by p.
                     * Accept it only if the whole value equals p
                     * (i.e. the number itself is the small prime). */
                    uint64_t num = num_base + v;
                    if (num == (uint64_t)p)
                        continue;   /* prime == small prime */
                    ok = false;   /* composite */
                    break;
                }
            }
        }
        good_residues[r] = ok ? 1 : 0;
    }
    good_residues_ready = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for 0 */

    if (!good_residues_ready)
        build_good_residues();

    /* Return cached value if available. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Search for new terms until we have at least n entries. */
    for (idx_t k = last_k + 1; ; ++k) {
        if (!good_residues[k % WHEEL_MOD])
            continue;   /* skip residue that fails small prime test */

        seq_t kk = (seq_t)k;
        seq_t P   = (seq_t)10 * kk * kk + 1;
        if (!isPrimeGeneric(P))
            continue;
        seq_t Q = P + 6;
        if (!isPrimeGeneric(Q))
            continue;
        Q = P + 12;
        if (!isPrimeGeneric(Q))
            continue;
        Q = P + 18;
        if (!isPrimeGeneric(Q))
            continue;

        /* Valid term found. Store it. */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = kk;
        last_k = k;

        if ((size_t)n <= cache_len)
            return cache[n - 1];
    }
}
