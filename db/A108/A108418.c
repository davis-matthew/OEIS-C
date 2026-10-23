/*
 * A108418  Primes with at least one of each odd digit and no even digits.
 * https://oeis.org/A108418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the OEIS A108418 “Primes with at least one
 * of each odd digit and no even digits”.
 * The algorithm tests successive odd integers, checks primality using
 * a deterministic Miller‑Rabin routine (valid for all 64‑bit values)
 * and verifies that each prime contains the set {1,3,5,7,9} and no
 * even digit.  The found primes are cached in a static array so that
 * repeated calls with arbitrary n are answered in O(1).  This
 * implementation satisfies the requirements for all usual build
 * configurations (uint8_t, uint16_t, …, unsigned __int128).  */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* deterministic Miller‑Rabin primality test for 64‑bit integers */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t result = 1;
    a %= mod;
    while (d) {
        if (d & 1) result = mul_mod64(result, a, mod);
        a = mul_mod64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    /* Miller‑Rabin with witnesses that are sufficient for 64‑bit range */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint64_t witnesses[] = {2,325,9375,28178,450775,9780504,1795265022};
    /* the above set is deterministic for 64‑bit integers */
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* mask for the five odd digits needed */
#define NEED_MASK  (uint16_t)0x02AAu  /* bits: 1,3,5,7,9 */

static bool prime_has_all_odd_digits(uint64_t p) {
    uint16_t seen = 0;
    while (p) {
        unsigned d = p % 10;
        p /= 10;
        if ((d & 1) == 0) return false;          /* even digit */
        seen |= 1u << d;
    }
    return (seen & NEED_MASK) == NEED_MASK;
}

/* --------------------------------------------------------------- */
/*   The required public function                                      */
/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t cand = 2;        /* next integer to test */

    /* Initialise cache lazily */
    if (cap == 0) {
        cap   = 1024;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* unlikely */
    }

    /* Return cached value if available */
    if (len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Generate terms until we reach index n */
    while (len < (size_t)n) {
        /* Find next prime */
        while (!is_prime_uint64(cand))
            ++cand;

        /* Check digit condition */
        if (prime_has_all_odd_digits(cand)) {
            /* Grow cache if necessary */
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* unlikely */
                cache = tmp;
            }
            cache[len++] = (seq_t)cand;
        }
        ++cand;   /* move to the next candidate */
    }

    return cache[(size_t)n - 1];
}
