/*
 * A076995  a(1) = 4, a(n+1) is the largest composite number < 2a(n).
 * https://oeis.org/A076995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A076995
 *
 * a(1) = 4
 * a(n+1) = the largest composite number < 2*a(n)
 *
 * Recurrence simplification:
 *   a(n+1) = 2*a(n) - 2  if 2*a(n)-1 is prime
 *   a(n+1) = 2*a(n) - 1  otherwise
 *   (since 2*a(n)-2 is always composite when 2*a(n)-1 is prime)
 *
 * The implementation memoizes all computed values so that calls
 * with arbitrary n are fast.  The only expensive operation is
 * the primality check of a potentially huge odd number; this
 * uses the provided is_prime_big() routine.
 */

#include <stdbool.h>
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

static seq_t *memo = NULL;     /* memoized a(1), a(2), ... */
static size_t memo_len = 0;    /* number of values stored */
static size_t memo_cap = 0;    /* allocated capacity */

/* Allocate or grow memo array to hold at least 'cap' elements. */
static void ensure_memo_capacity(size_t cap)
{
    if (cap <= memo_cap) return;
    size_t new_cap = memo_cap == 0 ? 1 : memo_cap * 2;
    while (new_cap < cap) new_cap *= 2;
    seq_t *new_mem = mem_resize(seq_t, memo, new_cap);
    if (!new_mem) {
        /* Out of memory: abort as no error handling is required. */
        abort();
    }
    memo = new_mem;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) { /* offset is 1: terms start at n=1 */
        return (seq_t)0;
    }

    /* Convert idx_t to seq_t for operations */
    seq_t idx_seq = (seq_t)n;

    /* Expand memo if necessary */
    if ((size_t)n > memo_len) {
        ensure_memo_capacity((size_t)n);
        /* Compute missing terms */
        for (size_t i = memo_len; i < (size_t)n; ++i) {
            if (i == 0) {
                memo[i] = (seq_t)4;                     /* a(1) */
                continue;
            }
            seq_t prev = memo[i - 1];
            seq_t candidate = (seq_t)2 * prev - (seq_t)1; /* 2*a(n-1) - 1 */

            if (is_prime_big(candidate)) {
                memo[i] = candidate - (seq_t)1;         /* 2*a-2 */
            } else {
                memo[i] = candidate;                    /* 2*a-1 */
            }
        }
        memo_len = (size_t)n;
    }

    return memo[(size_t)n - 1];
}
