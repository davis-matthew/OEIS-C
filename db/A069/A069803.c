/*
 * A069803  Smaller of two consecutive palindromic primes.
 * https://oeis.org/A069803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   grow_seq_array, is_palindrome, is_prime_big, next_prime
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
#include "../utils/is_prime_big.h"
#include "../utils/is_palindrome.h"
#include "../utils/next_prime.h"
#include "../utils/grow_seq_array.h"

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
            if (is_palindrome((seq_t)next, 10)) {
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
