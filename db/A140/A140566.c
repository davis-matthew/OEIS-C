/*
 * A140566  List of prime quartets (k - 11, k - 1, k + 1, k + 11).
 * https://oeis.org/A140566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the sequence formed by concatenating all prime *
 *            quadruples (k-11, k-1, k+1, k+11) for which all four numbers are
 *            * prime. Each quartet is appended in order k-11, k-1, k+1, k+11,
 *            * and quartets are processed in increasing k. The first quartet
 *            * occurs for k = 18 giving (7,17,19,29). The se...
 * Range:     u8 n <= 24, u16 n <= 420, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 984 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;   /* cached terms of the sequence */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 18;      /* next value of k to test (first candidate) */

static inline void grow_cache(size_t needed)
{
    if (cache_len + needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < cache_len + needed) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

static inline void append_to_cache(seq_t val)
{
    grow_cache(1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    size_t target = (size_t)n;
    while (cache_len < target) {
        /* Search for the next k that yields a valid quartet */
        while (true) {
            if (is_prime_big((seq_t)(next_k - 11)) &&
                is_prime_big((seq_t)(next_k - 1))  &&
                is_prime_big((seq_t)(next_k + 1))  &&
                is_prime_big((seq_t)(next_k + 11)))
            {
                break;
            }
            next_k += 2;   /* k must be even, start from the first even k */
        }

        /* Append the quartet in the required order */
        append_to_cache((seq_t)(next_k - 11));
        append_to_cache((seq_t)(next_k - 1));
        append_to_cache((seq_t)(next_k + 1));
        append_to_cache((seq_t)(next_k + 11));

        /* Advance k for the next search */
        next_k += 2;
    }
    return cache[n - 1];
}
