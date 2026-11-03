/*
 * A142152  Primes congruent to 21 mod 38.
 * https://oeis.org/A142152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 21 (mod 38)
 * Range:     u8 n <= 4, u16 n <= 361, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 61 terms through n = 2473 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by incrementally testing candidates of the
   form 38*k + 21.  The test uses the `is_prime_big` routine provided
   by the utilities library.  The result is memoised so that calls out
   of order are fast. */

#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *prime_list = NULL;  /* stored primes */
static size_t prime_count = 0;    /* how many were found */
static size_t prime_cap   = 0;    /* capacity of prime_list */
static idx_t   next_k      = 1;   /* next k to test (1 → first candidate 59) */

/* Grow the array so that one more prime can be stored */
static void ensure_capacity(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = mem_resize(seq_t, NULL, prime_cap);
    } else if (prime_count >= prime_cap) {
        prime_cap <<= 1;  /* double */
        prime_list = mem_resize(seq_t, prime_list, prime_cap);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 as a safe value */

    /* Grow the memoised list until we have at least n primes */
    while (prime_count < n) {
        ensure_capacity();

        /* Form candidate 38*k + 21 (all arithmetic in idx_t to avoid overflow) */
        idx_t cand_idx = (idx_t)38 * next_k + 21;
        seq_t candidate = (seq_t)cand_idx;

        if (is_prime_big(candidate)) {
            prime_list[prime_count++] = candidate;
        }
        ++next_k;
    }

    return prime_list[n - 1];
}
