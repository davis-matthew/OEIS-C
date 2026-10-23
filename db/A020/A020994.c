/*
 * A020994  Primes that are both left-truncatable and right-truncatable.
 * https://oeis.org/A020994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   this code searches for primes that remain prime when any number
 *            of leading or trailing digits are removed. For a prime p all
 *            truncations from the left and from the right are tested. The
 *            function builds the complete list once and then returns the n‑th
 *            element (1‑based). Beyond the known 15 terms i...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   build_primes_upto, digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

static seq_t *two_sided_cache = NULL;
static size_t two_sided_count = 0;
static bool cache_built = false;

/* Test whether a prime p is truncatable from both sides. */
static bool is_two_sided_prime(uint64_t p)
{
    seq_t sp = (seq_t)p;
    int len = digits(p);          /* number of decimal digits of p */
    for (int i = 1; i < len; ++i) {
        seq_t pow = pow10((idx_t)i);      /* 10^i */
        seq_t suffix = sp % pow;          /* last i digits */
        seq_t prefix = sp / pow;          /* first len-i digits */
        if (!is_prime_big(suffix) || !is_prime_big(prefix))
            return false;
    }
    return true;
}

/* Build the cache of all two‑sided truncatable primes. */
static void build_cache(void)
{
    if (cache_built) return;
    const uint64_t LIMIT = 1000000ULL;          /* enough to find all 15 terms */
    uint64_t *prime_array = NULL;
    size_t prime_cnt = 0;

    build_primes_upto(LIMIT, &prime_array, &prime_cnt);

    /* Allocate initial space for the results. */
    size_t cap = 16;
    two_sided_cache = (seq_t *)malloc(cap * sizeof(seq_t));
    two_sided_count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        if (is_two_sided_prime(p)) {
            if (two_sided_count == cap) {
                cap *= 2;
                two_sided_cache = (seq_t *)realloc(two_sided_cache,
                                                   cap * sizeof(seq_t));
            }
            two_sided_cache[two_sided_count++] = (seq_t)p;
        }
    }
    free(prime_array);
    cache_built = true;
}

/* External function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_cache();
    idx_t idx = n - 1;
    if ((size_t)idx >= two_sided_count) return 0;
    return two_sided_cache[idx];
}
