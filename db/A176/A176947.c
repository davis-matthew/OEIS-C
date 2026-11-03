/*
 * A176947  Primes of the form 11^k + 2^k.
 * https://oeis.org/A176947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, bref, more
 * Formula:   a(n) is the n-th prime of the form 11^k + 2^k (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Cached primes and state */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;          /* last k processed */
static seq_t pow11 = 1;           /* 11^k */
static seq_t pow2  = 1;           /* 2^k */
static bool initialized = false;
static const seq_t LIMIT = (seq_t)UINT64_MAX;

/* Initialise static state once. */
static void init_state(void)
{
    if (!initialized) {
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        last_k = 0;
        pow11 = 1;
        pow2 = 1;
        initialized = true;
    }
}

/* Extend cache until it contains at least n primes. */
static void extend_cache(idx_t n)
{
    init_state();

    while (cache_len < n) {
        seq_t sum = pow11 + pow2;
        if (is_prime_big(sum)) {
            if (cache_len >= cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = sum;
        }

        /* Prepare next powers.  Stop if further values would overflow
           the 64‑bit limit (only for 64‑bit builds). */
        if (sizeof(seq_t) <= 8 && pow11 > LIMIT / 11)
            break;
        pow11 *= 11;
        pow2 *= 2;
        last_k++;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offsets start at 1 */

    extend_cache(n);
    if (cache_len >= n)
        return cache[n - 1];
    return 0;                     /* value not found (e.g. beyond known terms) */
}
