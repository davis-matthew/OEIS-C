/*
 * A104995  Smallest prime such that n*k(n)^2+n*k(n)+1 is a prime >
 *          (n-1)*k(n-1)^2+(n-1)*k(n-1)+1 with k(n)>1 or 0 if n=4 as no prime
 *          possible.
 * https://oeis.org/A104995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104995
   a(n) = the smallest prime of the form n*k^2 + n*k + 1
           with k > 1 such that it is larger than the previous
           non‑zero term; if no such prime exists (n = 4) the value
           is 0. */

/* Core code – relies on a deterministic Miller–Rabin test for 64‑bit
   integers.  The sequence values for n ≤ 10000 are well below 2^64,
   so we cast to uint64_t for primality testing. */

/* ---- Standard headers ---- */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- 64‑bit modular arithmetic helpers ---- */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul64(r, a, m);
        a = mod_mul64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* ---- Deterministic Miller–Rabin for 64‑bit integers ---- */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* Helper for arbitrary‐precision types – cast to uint64_t. */
static inline bool is_prime_seq(seq_t x)
{
    return is_prime_uint64((uint64_t)x);
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    /* Memoisation storage. */
    static seq_t *store = NULL;       /* 0‑based: store[i] = a(i+1) */
    static size_t cap = 0;            /* capacity of store */
    static idx_t len = 0;             /* highest index already computed (>=1) */
    static seq_t last_prime = 0;      /* last non‑zero term for comparison */

    /* Expand storage if required. */
    if (n > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(store, newcap * sizeof(seq_t));
        if (!tmp) { abort(); }
        store = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n. */
    for (idx_t idx = len + 1; idx <= n; ++idx) {
        seq_t result = 0;  /* default for n = 4 */

        if (idx != 4) {
            for (idx_t k = 2; ; ++k) {
                unsigned __int128 prod =
                    (unsigned __int128)idx * k * k
                  + (unsigned __int128)idx * k
                  + 1;
                seq_t candidate = (seq_t)prod;
                if (is_prime_seq(candidate) && (idx == 1 || candidate > last_prime)) {
                    result = candidate;
                    break;
                }
            }
            if (result != 0) last_prime = result;
        }

        store[idx - 1] = result;
    }

    len = n;
    return store[n - 1];
}
