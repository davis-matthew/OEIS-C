/*
 * A069803  Smaller of two consecutive palindromic primes.
 * https://oeis.org/A069803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_palindrome_base, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069803: Smaller of two consecutive palindromic primes.
 * Computation: generate palindromic numbers in increasing order,
 * test primality, and if the next prime after a palprime is itself
 * a palindrome, store the palprime as a term.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Create a palindrome from the given left part and total length.
 * left is the first (ceil(len/2)) digits, len is the full length. */
static seq_t make_pal(uint64_t left, int len) {
    seq_t pal = left;
    uint64_t rev = left;
    if (len & 1)          /* odd length – skip the middle digit when mirroring */
        rev /= 10;
    while (rev) {
        pal = pal * 10 + (rev % 10);
        rev /= 10;
    }
    return pal;
}

/* State kept between calls: cache of already computed terms and
 * state of the palindrome generator. */
static seq_t *cache     = NULL;
static size_t cache_len = 0, cache_cap = 0;

static int  cur_len       = 1;          /* current palindrome length        */
static uint64_t left_start = 1;          /* smallest left for current length */
static uint64_t left_end   = 9;          /* largest  left for current length */
static uint64_t left_cur   = 1;          /* current left value               */

/* Generate terms until at least 'target' terms are available. */
static void generate_until(idx_t target) {
    while (cache_len < target) {
        seq_t pal = make_pal(left_cur, cur_len);            /* exact palindrome */
        /* test primality */
        if (is_prime_big(pal)) {
            uint64_t next = next_prime((uint64_t)pal);      /* next prime after pal */
            if (is_palindrome_base((seq_t)next, 10)) {
                if (cache_len == cache_cap)
                    grow_seq_array(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = pal;                  /* store the smaller prime */
            }
        }
        /* advance to next palindrome */
        ++left_cur;
        if (left_cur > left_end) {
            ++cur_len;
            /* recompute bounds for the new length */
            int half = (cur_len + 1) / 2;                /* ceil(cur_len/2)   */
            left_start = 1;
            for (int i = 1; i < half; ++i) left_start *= 10;
            left_end   = left_start * 10 - 1;
            left_cur   = left_start;
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    generate_until(n);
    return cache[n - 1];
}
