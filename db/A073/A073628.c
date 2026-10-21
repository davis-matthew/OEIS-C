/*
 * A073628  a(0) = 0; a(1) = 1; a(2) = 2; a(n) = smallest number greater than
 *          the previous term such that the sum of three successive terms is a
 *          prime.
 * https://oeis.org/A073628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A073628: a(0)=0, a(1)=1, a(2)=2;
    a(n)=smallest k > a(n‑1) such that a(n‑2)+a(n‑1)+k is prime.   */

/*  ==== Helpers ====================================================  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*  deterministic 64‑bit Miller–Rabin primality test   */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /*  write n‑1 as d·2ᵖ  */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    /*  witnesses that are sufficient for deterministic 64‑bit test   */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof witnesses / sizeof witnesses[0]; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, power = a;
        uint64_t r = d;
        /*  compute a^d mod n   */
        while (r) {
            if (r & 1) x = (x * power) % n;
            power = (power * power) % n;
            r >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t j = 1; j < s; ++j) {
            x = (x * x) % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/*  ==== Cache ==============================================================  */
static seq_t *cache = NULL;      /*  dynamic array of seq_t  */
static idx_t cache_size = 0;     /*  number of terms stored  */
static idx_t cache_cap   = 0;    /*  array capacity          */

static void ensure_cache_cap(idx_t need)
{
    if (cache_cap >= need) return;
    idx_t new_cap = (cache_cap > 0) ? (cache_cap * 3) / 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

static void compute_up_to(idx_t n)
{
    if (n < cache_size) return;
    ensure_cache_cap(n + 1);

    /*  initialise first three terms if necessary  */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_size = 3;
    } else if (cache_size == 1) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_size = 3;
    } else if (cache_size == 2) {
        cache[2] = (seq_t)2;
        cache_size = 3;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        uint64_t a_prev2 = (uint64_t)cache[i - 2];
        uint64_t a_prev1 = (uint64_t)cache[i - 1];
        uint64_t cand = a_prev1 + 1;
        while (true) {
            uint64_t sum = a_prev2 + a_prev1 + cand;
            if (is_prime_u64(sum)) break;
            ++cand;
        }
        cache[i] = (seq_t)cand;
    }
    cache_size = n + 1;
}

/*  ==== Public interface ===================================================  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    compute_up_to(n);
    return cache[n];
}
