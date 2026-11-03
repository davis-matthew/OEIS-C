/*
 * A134101  Odd nonprimes such that the prior odd number is not a prime but
 *          the next odd number is a prime.
 * https://oeis.org/A134101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 937 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate odd numbers, keeping those that satisfy
 * (odd non‑prime, previous odd non‑prime, next odd prime).  The
 * function memoises found terms, so calls may arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
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

/* Note: seq_t and idx_t are provided by the build environment.
 * They may be an unsigned integer type or an arbitrary‑precision
 * integer class.  This implementation works with either. */

seq_t a(idx_t n)
{
    /* Memoised cache for previously found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The next odd integer to examine (starting from 3). */
    static idx_t next_odd = 3;

    if (n < 1) {
        /* Undefined for n < 1; return 0 safely. */
        return (seq_t)0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Convert candidate odd numbers to seq_t for primality tests. */
        seq_t odd_seq  = (seq_t)next_odd;
        seq_t prev_seq = (seq_t)(next_odd - 2);
        seq_t next_seq = (seq_t)(next_odd + 2);

        /* Check the sequence condition. */
        if (!is_prime_big(odd_seq) &&
            !is_prime_big(prev_seq) &&
            is_prime_big(next_seq)) {

            /* Grow cache if it is full. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* Memory allocation failed; abort computation. */
                    return (seq_t)0;
                }
                cache     = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = odd_seq;

            /* If we have reached the requested element, stop scanning. */
            if (cache_len == (size_t)n) {
                /* Advance past the current odd for future calls. */
                next_odd += 2;
                break;
            }
        }

        /* Move to the next odd number. */
        next_odd += 2;
    }

    return cache[n - 1];
}
