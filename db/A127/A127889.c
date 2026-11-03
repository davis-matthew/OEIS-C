/*
 * A127889  Smallest n-digit right-truncatable prime.
 * https://oeis.org/A127889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   A127889 – smallest n‑digit right‑truncatable prime (no simple
 *            closed form; computed by incremental construction)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache for arbitrary order of calls */
    static bool initialized = false;
    static seq_t *seq = NULL;          /* seq[1 .. max] */
    static size_t seq_len = 0;         /* number of valid terms stored */
    static seq_t *cur  = NULL;         /* primes of current length */
    static size_t cur_sz = 0;
    static size_t cur_cap = 0;
    static seq_t *next = NULL;         /* primes of next length */
    static size_t next_sz = 0;
    static size_t next_cap = 0;

    /* illegal indices return 0 */
    if (n < 1) return 0;

    /* one‑time initialisation */
    if (!initialized) {
        unsigned seq_cap = 10001;               /* supports indices up to ~10 000 */
        seq = mem_new(seq_t, seq_cap);
        if (!seq) return 0;

        cur_cap = 16;
        cur = mem_new(seq_t, cur_cap);
        if (!cur) { mem_free(seq); return 0; }
        cur[0] = 2; cur[1] = 3; cur[2] = 5; cur[3] = 7;
        cur_sz = 4;

        next_cap = 16;
        next = mem_new(seq_t, next_cap);
        if (!next) { mem_free(seq); mem_free(cur); return 0; }

        /* the smallest 1‑digit right‑truncatable prime */
        seq[1] = 2;
        seq_len = 1;

        initialized = true;
    }

    /* generate terms until we have at least the requested n */
    for (size_t len = seq_len + 1; len <= (size_t)n; ++len) {
        /* build primes of length 'len' by appending a digit to each prime of length len-1 */
        next_sz = 0;
        for (size_t i = 0; i < cur_sz; ++i) {
            seq_t p = cur[i];
            for (int d = 0; d <= 9; ++d) {
                seq_t v = p * ((seq_t)10) + (seq_t)d;
                if (is_prime_big(v)) {
                    if (next_sz == next_cap) {
                        size_t new_cap = next_cap * 2;
                        seq_t *tmp = mem_resize(seq_t, next, new_cap);
                        if (!tmp) { mem_free(next); mem_free(cur); mem_free(seq); return 0; }
                        next = tmp;
                        next_cap = new_cap;
                    }
                    next[next_sz++] = v;
                }
            }
        }

        if (next_sz == 0) {
            /* no further right‑truncatable primes exist – fill remaining indices with 0 */
            for (size_t i = len; i <= (size_t)n; ++i) seq[i] = 0;
            return seq[n];
        }

        /* smallest prime of this length becomes a(len) */
        seq_t min_val = next[0];
        for (size_t i = 1; i < next_sz; ++i)
            if (next[i] < min_val) min_val = next[i];
        seq[len] = min_val;
        seq_len = len;

        /* swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_sz = next_sz;
        /* cur_cap and next_cap remain adequate for future growth */
    }

    return seq[n];
}
