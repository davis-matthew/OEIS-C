/*
 * A216307  Values of k such that 10*k+3 and 10*k+9 alone are prime between
 *          10*k and 10*k+9.
 * https://oeis.org/A216307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that 10*k+3 and 10*k+9 * are the
 *            only two primes in the interval [10*k , 10*k+9]. * * The
 *            function keeps a static cache of found k‑values so that * calls
 *            with arbitrary order of n do not recompute preceding terms. * It
 *            uses a deterministic Miller–Rabin test for 64‑bi...
 * Range:     u8 n <= 31, u16 n <= 2913, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- Helper: deterministic Miller–Rabin for 64‑bit --------- */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t res = 1;
    while (d) {
        if (d & 1) res = mod_mul_uint64(res, a, mod);
        a = mod_mul_uint64(a, a, mod);
        d >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* Decompose n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static bool is_prime(idx_t n) { return is_prime_uint64((uint64_t)n); }

/* ---------- Sequence computation ---------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset 1, but handle invalid 0 */

    /* static cache of found k values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;    /* number of stored k's */
    static idx_t cache_cap   = 0;    /* allocated capacity */
    static idx_t next_k      = 1;    /* next k candidate to test */

    /* extend cache until it contains at least n terms */
    while (cache_size < n) {
        /* test current candidate k = next_k */
        idx_t k = next_k;
        idx_t p3 = 10 * k + 3;
        idx_t p9 = 10 * k + 9;

        bool ok = true;
        if (is_prime(p3) && is_prime(p9)) {      /* primes at required positions */
            /* check the remaining six numbers in the block */
            for (idx_t offset = 1; offset <= 8; ++offset) {
                if (offset == 3 || offset == 9) continue; /* skip p3 and p9 */
                idx_t x = 10 * k + offset;
                if (is_prime(x)) { ok = false; break; }
            }
        } else {
            ok = false;
        }

        if (ok) {
            /* store found k */
            if (cache_size == cache_cap) {           /* need more room */
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)k;          /* implicit conversion to seq_t */
        }

        ++next_k;
    }

    return cache[n-1];
}
