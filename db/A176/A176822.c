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
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem
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
#include <stddef.h>
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

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

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
                                cache = mem_resize(seq_t, cache, cache_cap);
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
