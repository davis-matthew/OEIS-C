/*
 * A209932  Numbers k such that smallest digit of all divisors of k is 0.
 * https://oeis.org/A209932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_zero, has_zero_divisor_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS:A209932
    Numbers k such that smallest digit of all divisors of k is 0.
    The algorithm enumerates positive integers k and checks whether
    any divisor of k contains a decimal digit 0.  A divisor contains
    a 0‑digit iff its decimal representation has a '0' as its
    smallest digit, therefore the test is simply: does there exist
    a divisor d of k with a zero digit?  If yes, k is retained in
    the sequence.  The sequence is generated lazily and memoised
    so that any call a(n) with n ≥ 1 returns the correct term.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/contains_zero.h"
#include "../utils/has_zero_divisor_recursive.h"

/* Factorisation of a 64‑bit integer into primes, at most 20 distinct factors. */
static void factor_uint64(uint64_t n,
                          uint64_t primes[20],
                          uint8_t  exps [20],
                          size_t  *pcnt)
{
    size_t cnt = 0;
    uint64_t t = n;

    if (t % 2ULL == 0ULL) {
        primes[cnt] = 2ULL;
        exps[cnt]   = 0;
        while (t % 2ULL == 0ULL) { t /= 2ULL; exps[cnt]++; }
        cnt++;
    }
    for (uint64_t p = 3ULL; p * p <= t; p += 2ULL) {
        if (t % p == 0ULL) {
            primes[cnt] = p;
            exps[cnt]   = 0;
            while (t % p == 0ULL) { t /= p; exps[cnt]++; }
            cnt++;
        }
    }
    if (t > 1ULL) {
        primes[cnt] = t;
        exps[cnt]   = 1;
        cnt++;
    }
    *pcnt = cnt;
}

/* Does k have a divisor that contains a zero digit? */
static bool has_zero_divisor(uint64_t k)
{
    if (k == 0ULL) return false;   /* 0 has no positive divisors in this routine */

    uint64_t primes[20];
    uint8_t  exps[20];
    size_t  pcnt;
    factor_uint64(k, primes, exps, &pcnt);

    return has_zero_divisor_recursive(0, 1ULL, pcnt, primes, exps);
}

/* --------------------------------------------------------------------- */
/*  Memoised sequence generation                                           */

static seq_t* seq_cache = NULL;
static idx_t  seq_size  = 0;
static idx_t  seq_cap   = 0;
static uint64_t current_k = 0;

/* Ensure that seq_cache contains at least n terms (n ≥ 1). */
static void ensure_sequence_upto(idx_t n)
{
    if (seq_size >= n)
        return;

    if (seq_cache == NULL) {
        seq_cap = 64;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_size < n) {
        current_k++;                     /* test the next integer */
        if (has_zero_divisor(current_k)) {
            if (seq_size == seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_size++] = (seq_t)current_k;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_sequence_upto(n);
    return seq_cache[n - 1];
}
