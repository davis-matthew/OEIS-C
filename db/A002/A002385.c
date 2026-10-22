/*
 * A002385  Palindromic primes: prime numbers whose decimal expansion is a
 *          palindrome.
 * https://oeis.org/A002385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   search. Generate odd-length decimal palindromes in increasing
 *            order (plus 11, the only even-length one), keep those that pass
 *            a deterministic Miller-Rabin test, and memoize the results
 * Range:     u8 n <= 10, u16 n <= 70, u32 n <= 5953, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   pp_isprime, pp_mulmod, pp_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pp_isprime.h"
#include "../utils/pp_mulmod.h"
#include "../utils/pp_powmod.h"

static uint64_t *pp_cache = NULL;
static size_t pp_count = 0, pp_cap = 0;
static int pp_k = 1;          /* half-length: palindrome has 2k+1 digits */
static uint64_t pp_h = 0;     /* next left half to try */
static uint64_t pp_pk = 10;   /* 10^k */

static void pp_push(uint64_t v) {
    if (pp_count == pp_cap) {
        size_t nc = pp_cap ? pp_cap * 2 : 1024;
        pp_cache = (uint64_t *)realloc(pp_cache, nc * sizeof(uint64_t));
        pp_cap = nc;
    }
    pp_cache[pp_count++] = v;
}

seq_t a(idx_t n) {
    if (pp_count == 0) {
        pp_push(2); pp_push(3); pp_push(5); pp_push(7); pp_push(11);
        pp_k = 1; pp_pk = 10; pp_h = 10;
    }
    while (pp_count < n && pp_k <= 9) {
        uint64_t lead, p, t;
        if (pp_h >= pp_pk * 10) {
            pp_k++;
            pp_pk *= 10;
            pp_h = pp_pk;
            continue;
        }
        lead = pp_h / pp_pk;
        if ((lead & 1) == 0 || lead == 5) {
            pp_h = (lead + 1) * pp_pk;
            continue;
        }
        p = pp_h;
        t = pp_h / 10;
        while (t) { p = p * 10 + t % 10; t /= 10; }
        pp_h++;
        if (pp_isprime(p)) pp_push(p);
    }
    if (n == 0 || n > pp_count) return (seq_t)0;
    return (seq_t)pp_cache[n - 1];
}
