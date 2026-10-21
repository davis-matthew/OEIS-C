/*
 * A088572  Numbers n such that (2n+1)^2 - 2 is prime.
 * https://oeis.org/A088572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 6557, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088572: numbers n such that (2n+1)^2 – 2 is prime.
   The function uses a simple search with a cached list of results.
   For seq_t sizes up to 64 bits a fast deterministic Miller–Rabin test
   is employed; for larger seq_t types a generic big‑integer test is
   used. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* ---------- fast 64‑bit primality test ----------------------------- */
/* Deterministic Miller–Rabin for values < 3 474 749 660 383,
   which comfortably covers all testable numbers in this sequence. */
static bool is_prime_seq(seq_t n)
{
    if (sizeof(seq_t) <= 8) {
        uint64_t un = (uint64_t)n;
        if (un < 2) return false;

        /* small primes first */
        static const uint64_t small_primes[] =
            {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
        for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
            if (un % small_primes[i] == 0) return un == small_primes[i];
        }

        /* factor n-1 as d * 2^s */
        uint64_t d = un - 1;
        int s = 0;
        while ((d & 1ULL) == 0) { d >>= 1; ++s; }

        /* bases adequate for < 3·10^12 */
        static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
        for (size_t bi = 0; bi < sizeof(bases)/sizeof(bases[0]); ++bi) {
            uint64_t a = bases[bi];
            if (a % un == 0) continue;

            /* modular exponentiation: x = a^d mod un */
            uint64_t x = 1, pow = a % un;
            uint64_t exp = d;
            while (exp) {
                if (exp & 1ULL) x = (unsigned __int128)x * pow % un;
                pow = (unsigned __int128)pow * pow % un;
                exp >>= 1;
            }

            if (x == 1 || x == un - 1) continue;

            bool composite = true;
            for (int r = 1; r < s; ++r) {
                x = (unsigned __int128)x * x % un;
                if (x == un - 1) { composite = false; break; }
            }
            if (composite) return false;
        }
        return true;
    } else {
        /* arbitrary precision fallback */
        return is_prime_big(n);
    }
}

/* ---------- sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0;                /* offset is 1; n<1 not used */

    while (cache_len < n) {
        idx_t start = (cache_len == 0) ? 1
                                        : (idx_t)cache[cache_len-1] + 1;

        for (idx_t cand = start; ; ++cand) {
            seq_t x = (seq_t)2 * (seq_t)cand + 1;
            seq_t p = x * x - 2;

            if (is_prime_seq(p)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2 + 1;
                    grow_seq(&cache, &cache_cap, new_cap);
                }
                cache[cache_len++] = (seq_t)cand;
                break;
            }
        }
    }
    return cache[n-1];
}
