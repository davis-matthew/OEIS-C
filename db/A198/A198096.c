/*
 * A198096  n such that d(1)*1! + d(2)*2! + ... + d(k)*k! is prime, where d(i)
 *          are the decimal digits of n.
 * https://oeis.org/A198096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198096
   a(n) = the n-th integer m such that
   sum_{i=1}^{k} d(i)*i!   is prime,
   where d(i) are the decimal digits of m (MSD first) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* Next candidate to test */
    static seq_t next_candidate = 2;

    /* Ensure the cache contains at least n terms */
    while (cache_size < n) {
        seq_t m = next_candidate++;         /* candidate number */

        /* ------------------------------------------------------------------
           Compute the digit array of m (least‑significant digit first)
           ------------------------------------------------------------------ */
        seq_t temp = m;
        static uint8_t digits[4096];          /* big enough for typical terms */
        size_t len = 0;
        if (temp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (temp > 0) {
                digits[len++] = (uint8_t)(temp % 10);
                temp /= 10;
            }
        }

        /* ------------------------------------------------------------------
           Compute sum = d(1)*1! + d(2)*2! + ... + d(k)*k!
           Here d(1) is the most significant digit, so we traverse digits
           from the end of the array.
           ------------------------------------------------------------------ */
        seq_t fact = 1; /* factorial accumulator */
        seq_t sum  = 0;
        for (size_t i = 1; i <= len; ++i) {
            fact *= (seq_t)i;                            /* i!  */
            sum += (seq_t)digits[len - i] * fact;      /* d(i)*i! */
        }

        /* ------------------------------------------------------------------
           Test primality of the sum
           ------------------------------------------------------------------ */
        if (is_prime_big(sum)) {
            /* Grow cache if needed */
            if (cache_size >= cache_cap) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
            }
            cache[cache_size++] = m;
        }
    }

    return cache[n - 1];
}
