/*
 * A119911  Natural numbers with number of divisors not equal to a Lucas
 *          number.
 * https://oeis.org/A119911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   natural numbers whose number of divisors is not a Lucas number
 *            Sequence A119911
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* ---------- Helpers ---------- */

/* Number of positive divisors of integer n (uint64_t). */
static uint32_t divisor_count_u64(uint64_t n)
{
    uint32_t d = 1;
    uint32_t exp = 0;

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        ++exp;
        n >>= 1;
    }
    if (exp)
        d *= (exp + 1);

    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                ++exp;
                n /= p;
            }
            d *= (exp + 1);
        }
    }
    if (n > 1)           /* remaining prime factor */
        d *= 2;
    return d;
}

/* Table of Lucas numbers up to a chosen limit.  The highest divisor
   count we encounter for n up to about 1e6 is < 2000, so a limit of
   4096 is safe. */
static bool lucas_table[4096];
static bool lucas_initialized = false;

static void init_lucas(void)
{
    if (lucas_initialized) return;
    lucas_initialized = true;
    uint64_t a = 2, b = 1;
    if (a < sizeof lucas_table) lucas_table[a] = true;
    if (b < sizeof lucas_table) lucas_table[b] = true;
    while (true) {
        uint64_t c = a + b;
        if (c >= sizeof lucas_table) break;
        lucas_table[c] = true;
        a = b;
        b = c;
    }
}

/* ---------- Sequence cache ---------- */

/* Cached sequence values */
static seq_t *seq_cache = NULL;   /* array of seq_t */
static size_t seq_cap   = 0;      /* allocated capacity */
static size_t seq_len   = 0;      /* number of elements stored */

/* The smallest natural number that has not yet been tested. */
static uint64_t next_candidate = 2;

/* Append a value to the cache, reallocating as necessary. */
static void push_value(seq_t value)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 32 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)seq_alloc_array(new_cap);
        if (seq_cache) {
            memcpy(new_arr, seq_cache, seq_len * sizeof(seq_t));
            seq_free_array(seq_cache);
        }
        seq_cache = new_arr;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = value;
}

/* Build the cache until it contains at least 'target' terms. */
static void build_cache(idx_t target)
{
    while (seq_len < target) {
        uint32_t d = divisor_count_u64(next_candidate);
        if (!lucas_table[d]) {
            push_value((seq_t)next_candidate);
        }
        ++next_candidate;
    }
}

/* ---------- Required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */
    init_lucas();
    if (seq_len < n) build_cache(n);
    return seq_cache[n - 1];
}
