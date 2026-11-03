/*
 * A178549  a(n) is a composite number at the start of an interval of
 *          consecutive integers, ending in a prime, and non-overlapping with
 *          and at least as long as the interval addressed by a(n-1).
 * https://oeis.org/A178549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: each a(n) is the start of the n-th non‑overlapping
 * interval of consecutive integers that begins with a composite number,
 * ends with a prime number, and whose length is at least as large as the
 * length of the previous interval.  The intervals are constructed
 * greedily and stored for fast repeated evaluation. */

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
    if (n == 0) return 0;          // offset is 1; values for n<1 are undefined

    /* Dynamically grow these arrays as needed. */
    static seq_t *starts   = NULL; /* a(k) values (start of each interval) */
    static idx_t *lengths = NULL; /* corresponding interval lengths */
    static idx_t stored   = 0;     /* number of entries already computed */
    static idx_t capacity = 0;     /* current allocation size */

    /* Ensure enough space for at least n terms. */
    if (n > stored) {
        if (n > capacity) {
            idx_t newcap = capacity ? capacity * 2 : 1;
            if (newcap < n) newcap = n;
            seq_t *tmp1 = mem_resize(seq_t, starts, newcap);
            if (!tmp1) abort();
            starts   = tmp1;
            idx_t *tmp2 = (idx_t*)realloc(lengths, newcap * sizeof(idx_t));
            if (!tmp2) abort();
            lengths = tmp2;
            capacity = newcap;
        }

        /* Compute missing terms up to n. */
        for (idx_t i = stored; i < n; ++i) {
            if (i == 0) {               /* a(1) = 4, length 2  */
                starts[0]   = 4;
                lengths[0]  = 2;
                continue;
            }

            seq_t prev_start = starts[i-1];
            idx_t  prev_len  = lengths[i-1];
            seq_t  start     = prev_start + (seq_t)prev_len; /* next interval start */

            /* Find the first prime at or beyond start + prev_len - 1. */
            seq_t candidate = start + (seq_t)prev_len - 1;
            while (!is_prime_big(candidate)) ++candidate;

            seq_t  end = candidate;
            seq_t  len = end - start + 1;          /* interval length */

            starts[i]  = start;
            lengths[i] = (idx_t)len;                /* len fits in idx_t for all n considered */
        }
        stored = n;
    }

    return starts[n-1];
}
