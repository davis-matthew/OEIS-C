/*
 * A060290  Primes which are sums of twin Harshad numbers (includes overlaps).
 * https://oeis.org/A060290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9409 (2026-11-03)
 * Helpers:   digit_sum_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060290: primes p such that (p-1)/2 and (p+1)/2 are Harshad numbers.
   The function computes the nth term for n ≥ 1. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum_idx.h"
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

/* Cached primes that satisfy the condition. */
static seq_t *prime_list = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;
static idx_t  next_k      = 0;   /* next k (≥1) to test */

/* Ensure the cache can hold at least 'needed' primes. */
static void grow_prime_list(void) {
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 64;
        seq_t *tmp = mem_resize(seq_t, prime_list, new_cap);
        if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Add a new prime to the cache. */
static void add_prime(seq_t p) {
    grow_prime_list();
    prime_list[prime_len++] = p;
}

/* Generate primes until at least 'n' terms are found. */
static void ensure_n_primes(idx_t n) {
    while (prime_len < n) {
        ++next_k;                       /* start testing from 1 */
        idx_t k  = next_k;
        idx_t k1 = k + 1;

        idx_t ds_k  = digit_sum_idx(k);
        if (ds_k == 0 || k % ds_k != 0) continue;

        idx_t ds_k1 = digit_sum_idx(k1);
        if (ds_k1 == 0 || k1 % ds_k1 != 0) continue;

        seq_t sum = (seq_t)k + (seq_t)k1;   /* sum = 2k+1 */
        if (is_prime_big(sum))
            add_prime(sum);
    }
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 for safety */
    ensure_n_primes(n);
    return prime_list[n - 1];
}
