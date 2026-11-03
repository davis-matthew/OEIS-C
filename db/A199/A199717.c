/*
 * A199717  Numbers k such that 6*k-1 is composite, but 6*k-5 is prime.
 * https://oeis.org/A199717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 999 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199717: numbers k such that 6*k-1 is composite but 6*k-5 is prime.
   Computed by linear search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *seq_cache = NULL;   /* cached k-values, 0‑based          */
static idx_t  seq_len   = 0;      /* number of cached values          */
static idx_t  seq_cap   = 0;      /* current capacity of seq_cache    */
static idx_t  k_next    = 0;      /* last tested k                    */

/* Grow the cache to hold at least 'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = mem_resize(seq_t, seq_cache, new_cap);
    if (!new_cache)
        abort();
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/* Return the n‑th element of the sequence (a(1)=6).  Undefined for n<1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n<1 (offset 1) */

    /* Extend cache until we have at least n values. */
    while (seq_len < n) {
        ++k_next;                /* next candidate k           */
        seq_t m2 = (seq_t)6 * k_next - 5;   /* 6*k-5  (prime test)   */
        seq_t m1 = (seq_t)6 * k_next - 1;   /* 6*k-1  (composite test) */

        /* 6*k-5 must be prime and 6*k-1 composite. */
        if (is_prime_big(m2) && !is_prime_big(m1)) {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len] = k_next;
            ++seq_len;
        }
    }

    return seq_cache[n - 1];
}
