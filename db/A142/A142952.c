/*
 * A142952  Primes congruent to 59 mod 64.
 * https://oeis.org/A142952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A142952 – primes congruent to 59 (mod 64).  */
/*  The function returns the n‑th prime p with p ≡ 59 (mod 64).  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* ---------------------  primality test for 64‑bit values  --------------------- */

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin for 64‑bit integers (bases 2,3,5,7,11,13) */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* test a^d ≡ 1 or ≡ -1 mod n, or a^(d*2^r) ≡ -1 for some r */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------------------  sequence storage and generation  --------------------- */

static seq_t *prime_cache = NULL;   /* array of found primes (as seq_t)   */
static size_t   cache_size  = 0;     /* number of primes stored           */
static size_t   cache_cap   = 0;     /* allocated capacity (in elements)  */

/* enlarge the cache to hold at least 'needed' primes */
static void grow_cache(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) { /* allocation failure – abort */ abort(); }
    if (prime_cache) {
        for (size_t i = 0; i < cache_size; ++i) new_arr[i] = prime_cache[i];
        free(prime_cache);
    }
    prime_cache = new_arr;
    cache_cap   = new_cap;
}

/* ----------------------------------------------------------------------------- */
/*  public function  */
/* ----------------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* undefined for offset 0, harmless */

    /* generate more primes if needed */
    while (cache_size < (size_t)n) {
        /* start from the previous prime + 64, or from 59 if none yet */
        uint64_t candidate = (cache_size == 0) ? 59ULL
                                                : (uint64_t)prime_cache[cache_size - 1] + 64ULL;

        /* find the next prime ≡ 59 (mod 64) */
        while (!is_prime_u64(candidate)) candidate += 64ULL;

        grow_cache(cache_size + 1);
        prime_cache[cache_size] = (seq_t)candidate;
        ++cache_size;
    }
    return prime_cache[n - 1];
}
