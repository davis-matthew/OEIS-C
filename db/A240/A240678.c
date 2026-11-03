/*
 * A240678  Primes p such that p*10+k is prime for exactly one value of the
 *          digit k.
 * https://oeis.org/A240678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 447, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 951 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A240678 sequence.
   For each prime p, count how many numbers p*10+k (k = 0..9) are prime.
   Exactly one such number must be prime to include p in the sequence. */
/* The function returns the n-th term (1‑based). The sequence is built
   incrementally and cached so that calls with arbitrary n work. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* Static cache of found sequence terms */
    static seq_t *buf   = NULL;
    static size_t buf_cap = 0;
    static size_t buf_cnt = 0;
    /* Next integer to test for primality (odd numbers only after 2) */
    static uint64_t next_candidate = 2;

    if (n == 0) return (seq_t)0;  /* undefined but safe guard */

    /* Keep searching until we have at least n terms in the cache */
    while (buf_cnt < n) {
        /* Find the next prime candidate */
        uint64_t cand = next_candidate;
        if (cand < 2) cand = 2;
        while (!is_prime_big((seq_t)cand)) {
            /* Skip even numbers after 2 */
            cand += (cand == 2 ? 1 : 2);
        }

        /* cand is a prime */
        seq_t p = cand;

        /* Count how many of p*10+k are prime for k=0..9 */
        int prime_digit_count = 0;
        for (int d = 0; d <= 9; ++d) {
            seq_t num = p * 10 + d;
            if (is_prime_big(num)) {
                ++prime_digit_count;
                if (prime_digit_count > 1) break;
            }
        }

        /* If exactly one digit works, store this prime */
        if (prime_digit_count == 1) {
            if (buf_cnt >= buf_cap) {
                size_t new_cap = (buf_cap == 0) ? 16 : buf_cap * 2;
                /* Realloc must be cast for C++ compatibility */
                seq_t *tmp = mem_resize(seq_t, buf, new_cap);
                if (!tmp) abort();  /* allocation failure */
                buf = tmp;
                buf_cap = new_cap;
            }
            buf[buf_cnt++] = p;
        }

        /* Next candidate to test (odd numbers only) */
        next_candidate = (cand == 2) ? 3 : cand + 2;
    }

    return buf[n - 1];
}
