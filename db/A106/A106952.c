/*
 * A106952  Primes of the form 3x^2-3xy+7y^2, with x and y nonnegative.
 * https://oeis.org/A106952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9977 (2026-11-03)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106952: primes of the form 3*x^2-3*x*y+7*y^2 with x,y≥0
   (searched by exhaustion, sorted, and unique). */
/* The routine generates all values ≤ a current bound, keeps the
   primes in a static array and rescales the bound until the desired
   index is reached.  For n≥10000 a bound of about 5×10⁵ is sufficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mulmod.h"

/* Miller‑Rabin for 64‑bit integers ------------------------------------- */

static inline uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/* static storage (enough for n up to ~10 000)                     */
static uint64_t *primes64 = NULL;         /* every prime of the form <= current_limit */
static size_t prime_cnt = 0;
static uint64_t current_limit = 200000;   /* initial search bound */
static bool initialized = false;

/* compare function for qsort ------------------------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *va = (const uint64_t*)a;
    const uint64_t *vb = (const uint64_t*)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* generate all primes of the form ≤ limit -------------------------- */
static void generate_primes(uint64_t limit)
{
    size_t cnt = 0, cap = 1024;
    free(primes64);
    primes64 = (uint64_t *)malloc(cap * sizeof *primes64);
    if (!primes64) abort();
    /* for fixed y the form is smallest, 6.25 y^2, at x = y/2, and it falls
       for x below that and rises after */
    for (uint64_t y = 0; 25 * y * y <= 4 * limit; ++y) {
        uint64_t baseY = 7ULL * y * y;
        for (uint64_t x = 0; ; ++x) {
            uint64_t val = 3ULL * x * x + baseY - 3ULL * x * y;
            if (val > limit) {
                if (2 * x >= y) break;
                continue;
            }
            if (val >= 2 && is_prime64(val)) {
                if (cnt == cap) {
                    uint64_t *t = (uint64_t *)realloc(primes64, (cap *= 2) * sizeof *primes64);
                    if (!t) abort();
                    primes64 = t;
                }
                primes64[cnt++] = val;
            }
        }
    }
    /* sort and remove duplicates */
    if (cnt > 1) qsort((void*)primes64, cnt, sizeof(uint64_t), cmp_uint64);
    size_t unique = 0;
    uint64_t prev = 0;
    bool first = true;
    for (size_t i = 0; i < cnt; ++i) {
        if (first || primes64[i] != prev) {
            primes64[unique++] = primes64[i];
            prev = primes64[i];
            first = false;
        }
    }
    prime_cnt = unique;
}

/* ensure we have at least n primes -------------------------------- */
static void ensure_n(idx_t n)
{
    while (prime_cnt < (size_t)n) {
        current_limit *= 2;
        generate_primes(current_limit);
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* offset 1 */
    if (!initialized) {
        generate_primes(current_limit);
        initialized = true;
    }
    ensure_n(n);
    return (seq_t)primes64[n-1];
}
