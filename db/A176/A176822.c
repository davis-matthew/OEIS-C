/*
 * A176822  Primes p with at least one decimal digit 2 that remain prime after
 *          removing all 2's.
 * https://oeis.org/A176822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big, seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176822: Primes containing at least one digit 2 that
   remain prime after deleting all 2's.  The function searches for
   successive primes, tests the condition using decimal string
   manipulation, and memoises the results.  It works for any
   integer size chosen for seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE 128

/* external utility functions available in the build */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/seq_to_str.h"    /* void seq_to_str(seq_t v, char *buf, size_t *len) */
#include "../utils/seq_from_str.h"  /* seq_t seq_from_str(const char *s) */

static seq_t *cache = NULL;   /* memoisation array */
static size_t cache_len = 0;   /* number of cached terms */
static size_t cache_cap = 0;   /* capacity of cache array */
static uint64_t next_candidate = 2;  /* integer next to test for primality */

static void ensure_cache(size_t upto)
{
    if (upto <= cache_len)
        return;
    for (size_t need = cache_len + 1; need <= upto; ++need) {
        uint64_t cand = next_candidate;
        /* search for the next admissible prime */
        while (1) {
            seq_t cand_seq = (seq_t)cand;
            if (is_prime_big(cand_seq)) {
                /* test if the decimal representation contains at least one '2' */
                char buf[BUF_SIZE];
                size_t len = 0;
                seq_to_str(cand_seq, buf, &len);
                bool has2 = false;
                for (size_t i = 0; i < len; ++i)
                    if (buf[i] == '2') { has2 = true; break; }
                if (has2) {
                    /* build the number obtained by deleting all '2' digits */
                    char rembuf[BUF_SIZE];
                    size_t r = 0;
                    for (size_t i = 0; i < len; ++i)
                        if (buf[i] != '2') rembuf[r++] = buf[i];
                    if (r > 0) {
                        rembuf[r] = '\0';
                        seq_t rem_seq = seq_from_str(rembuf);
                        if (rem_seq != (seq_t)0 && is_prime_big(rem_seq)) {
                            /* admissible prime found */
                            if (cache_len + 1 > cache_cap) {
                                cache_cap = cache_cap ? cache_cap * 2 : 16;
                                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                            }
                            cache[cache_len++] = cand_seq;
                            next_candidate = cand + 1;
                            break;
                        }
                    }
                }
            }
            ++cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, behaviour for n<1 undefined */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
