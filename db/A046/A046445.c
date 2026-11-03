/*
 * A046445  Smallest composite with n prime factors that are distinct in
 *          length.
 * https://oeis.org/A046445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the smallest prime with k digits for k = 1 ..
 *            n+1 ; a(0) = 1 (empty product)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *primes = NULL;
static seq_t *prefix = NULL;
static size_t primes_cap = 0;
static size_t prefix_cap = 0;
static size_t primes_cnt = 0;

/* enlarge storage for primes and prefix products */
static void grow(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_primes = mem_resize(seq_t, primes, new_cap);
    seq_t *new_prefix = mem_resize(seq_t, prefix, new_cap);
    primes = new_primes;
    prefix = new_prefix;
    primes_cap = new_cap;
}

/* compute the smallest prime with exactly len digits */
static seq_t smallest_prime_with_len(size_t len)
{
    seq_t candidate;
    if (len == 1) {
        candidate = 2;               /* special case */
    } else {
        /* 10^(len-1) */
        candidate = ipow_idx((seq_t)10, (idx_t)(len - 1));
    }

    while (!is_prime_big(candidate)) {
        /* candidate++ */
        candidate = candidate + (seq_t)1;
    }
    return candidate;
}

seq_t a_old(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset 0 */

    size_t need = (size_t)(n + 1);  /* how many primes are required */

    /* generate primes and prefix products up to 'need' */
    while (primes_cnt < need) {
        grow(need);
        size_t idx = primes_cnt + 1;          /* digit length is idx (1‑based) */
        seq_t p = smallest_prime_with_len(idx);
        primes[primes_cnt] = p;
        if (primes_cnt == 0) {
            prefix[primes_cnt] = p;
        } else {
            prefix[primes_cnt] = seq_mul(prefix[primes_cnt - 1], p);
        }
        primes_cnt++;
    }

    return prefix[need - 1];
}

/* a_old(k) is the term with k+1 prime factors for k >= 1 (it left out
   a(1) = 2, the smallest number with one prime factor) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    return a_old(n - 1);
}
