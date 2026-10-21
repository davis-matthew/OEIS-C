/*
 * A062832  Numbers k such that k and k+2 have the same number of divisors.
 * https://oeis.org/A062832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that the number of divisors of k
 *            equals the number of divisors of k+2 (OEIS A062832)
 * Range:     u8 n <= 54, u16 n <= 9024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) */
#include "../utils/ensure_capacity.h"    /* ensure_capacity(size_t, size_t*, void**, size_t) */

static seq_t *seq_cache = NULL;      /* array of found terms */
static size_t seq_len   = 0;         /* number of terms stored so far */
static size_t seq_cap   = 0;         /* current allocation capacity */
static uint64_t next_k   = 1;        /* next integer to test */

static void add_term(uint64_t k) {
    if (seq_len == seq_cap) {
        /* grow the cache to hold at least one more element */
        ensure_capacity(seq_len + 1, &seq_cap, (void**)&seq_cache, sizeof(seq_t));
    }
    seq_cache[seq_len++] = (seq_t)k;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for the offset, guard against accidental call */

    /* keep generating terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t d1 = divisor_count_u64(next_k);
        uint64_t d2 = divisor_count_u64(next_k + 2);
        if (d1 == d2) {
            add_term(next_k);
        }
        next_k++;
    }

    return seq_cache[(size_t)n - 1];
}
