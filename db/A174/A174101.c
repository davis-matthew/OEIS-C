/*
 * A174101  Numbers k such that 2*k+1 and 6*k+1 are both primes of the form
 *          6*m+1.
 * https://oeis.org/A174101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 889 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer k such that 2*k+1 and 6*k+1 are both primes of the form 6*m+1 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* Static cache for generated terms */
static seq_t cache[20000];
static size_t cache_size = 0;
static idx_t current_k = 3;   /* next candidate multiple of 3 */

/* Generate terms until the cache contains at least n terms */
static void generate_up_to(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    while (cache_size < (size_t)n)
    {
        seq_t p1 = (seq_t)current_k * 2 + 1;
        seq_t p2 = (seq_t)current_k * 6 + 1;

        if (is_prime_big(p1) && is_prime_big(p2))
        {
            cache[cache_size++] = current_k;
        }
        current_k += 3;   /* next multiple of 3 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n = 0, return 0 for safety */
        return 0;
    generate_up_to(n);
    return cache[(size_t)n - 1];
}
