/*
 * A096012  Numbers k such that k^2+1 and (k+2)^2+1 are both prime; twin k^2+1
 *          primes.
 * https://oeis.org/A096012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th even integer k such that k²+1 and (k+2)²+1 are
 *            both prime, with a(1)=2. The sequence is computed by generating
 *            candidates in order, testing primality with a fast deterministic
 *            Miller–Rabin routine and a small‑prime filter, and caching
 *            results for repeated calls
 * Range:     u8 n <= 7, u16 n <= 343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Miller–Rabin helpers --------------------------------------------------- */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mul_mod(res, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64–bit integers (bases enough for all n < 2^64). */
static bool miller_rabin(uint64_t n) {
    if (n < 2) return false;
    /* Trial division by small primes up to 997. */
    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
        67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,
        149,151,157,163,167,173,179,181,191,193,197,199,211,223,
        227,229,233,239,241,251,257,263,269,271,277,281,283,293,
        307,311,313,317,331,337,347,349,353,359,367,373,379,383,
        389,397,401,409,419,421,431,433,439,443,449,457,461,463,
        467,479,487,491,499,503,509,521,523,541,547,557,563,569,
        571,577,587,593,599,601,607,613,617,619,631,641,643,647,
        653,659,661,673,677,683,691,701,709,719,727,733,739,743,
        751,757,761,769,773,787,797,809,811,821,823,827,829,839,
        853,857,859,863,877,881,883,887,907,911,919,929,937,941,
        947,953,967,971,977,983,991,997
    };
    size_t i;
    for (i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
        if ((uint64_t)p * p > n) break;
    }

    /* Miller–Rabin with deterministic bases for 64-bit integers. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    size_t j;
    for (j = 0; j < sizeof(bases) / sizeof(bases[0]); ++j) {
        uint64_t a = bases[j] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        int r;
        for (r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --- Sequence generation ---------------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static uint64_t next_k = 2;  /* next even candidate to test */

/* Ensure the cache has at least n elements; returns the polled array. */
static void ensure_cache(idx_t n) {
    if (cache_len >= n) return;
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
    }
    while (cache_len < n) {
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!cache) exit(1);
        }

        uint64_t kk = next_k;
        uint64_t p1 = (unsigned __int128)kk * kk + 1;
        uint64_t p2 = (unsigned __int128)(kk + 2) * (kk + 2) + 1;

        if (miller_rabin(p1) && miller_rabin(p2)) {
            cache[cache_len] = (seq_t)kk;
            cache_len++;
        }
        next_k += 2;  /* only even k can work */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    ensure_cache(n);
    return cache[n - 1];
}
