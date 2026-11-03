/*
 * A058049  Numbers k such that the sum of the digits of the first k primes is
 *          a prime.
 * https://oeis.org/A058049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9908 (2026-11-03)
 * Helpers:   digit_sum_u64_to_u32, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A058049: Numbers k such that the sum of the digits of the first k primes is a prime.
   Implementation: sequentially generate primes, keep a running sum of their digit sums,
   and record the prime count whenever that sum is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/digit_sum_u64_to_u32.h"
#include "../utils/next_prime.h"
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

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(1) is the first term */
        return 0;

    /* ---------- static state persist between calls ---------- */
    static seq_t *terms = NULL;        /* array of sequence terms (k values) */
    static size_t terms_cap = 0;        /* current capacity of terms array */
    static size_t terms_len = 0;        /* number of terms already computed */

    static seq_t digit_sum = 0;         /* cumulative sum of digits of processed primes */
    static idx_t prime_cnt = 0;         /* count of primes processed so far */
    static uint64_t cur_prime = 1;      /* last prime found (next_prime(1) -> 2) */

    size_t target = (size_t)n;         /* needed number of terms */

    /* ---- grow terms array if necessary ---- */
    if (terms_cap < target) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }

    /* ---- generate terms until we have at least `target` terms ---- */
    while (terms_len < target) {
        cur_prime = next_prime(cur_prime);
        prime_cnt++;

        /* add digit sum of the new prime to the running total */
        digit_sum += (seq_t)digit_sum_u64_to_u32(cur_prime);

        /* if the cumulative digit sum is prime, record the current prime count */
        if (is_prime_big(digit_sum)) {
            if (terms_len == terms_cap) {   /* grow array if we hit capacity */
                size_t new_cap = terms_cap << 1;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) exit(1);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)prime_cnt;
        }
    }

    return terms[n - 1];
}
