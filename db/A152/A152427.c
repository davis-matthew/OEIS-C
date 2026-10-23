/*
 * A152427  Primes that have both prime digits (2,3,5,7) and nonprime digits
 *          (1,4,6,8,9).
 * https://oeis.org/A152427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that contain at least one prime digit (2,3,5,7)
 * and at least one nonprime digit (1,4,6,8,9).
 * Computation is performed by incremental prime search and digit test. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool has_both_digits(uint64_t n)
{
    bool has_prime = false, has_nonprime = false;
    while (n) {
        unsigned d = n % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7) has_prime = true;
        else if (d == 1 || d == 4 || d == 6 || d == 8 || d == 9) has_nonprime = true;
        if (has_prime && has_nonprime) return true;
        n /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* -------- static memoisation state -------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0;
    static size_t prime_count = 0;

    static uint64_t next_candidate = 2;

    /* -------- conversion of index -------- */
    if (n == 0) return 0;           /* offset not requested */
    size_t target = (size_t)n;

    /* -------- extend cache until it contains the desired index -------- */
    while (cache_len < target) {
        uint64_t cand = next_candidate;

        if (cand <= 2) {
            next_candidate = 3;       /* after checking 2, jump to 3 */
        } else {
            next_candidate += 2;      /* skip even numbers > 2 */
        }

        /* test primality of cand */
        bool is_prime = true;
        if (cand < 2) is_prime = false;
        else if (cand == 2) is_prime = true;
        else {
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t p = prime_list[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }
        }
        if (!is_prime) continue;

        /* store prime in prime_list */
        if (prime_count == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 8;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
            prime_list = tmp;
            prime_cap = new_cap;
        }
        prime_list[prime_count++] = cand;

        /* test digit condition and cache if satisfied */
        if (has_both_digits(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[target - 1];
}
