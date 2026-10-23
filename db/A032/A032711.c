/*
 * A032711  Numbers k such that k prefixed by '2' and followed by '3' is
 *          prime.
 * https://oeis.org/A032711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 302, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, is_prime_big, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = k where k ≥ 1 and the decimal number 2‖k‖3 is prime,
 * with ‖ reading meaning concatenation.  For example, k = 8 gives 283.
 */

/* include required helpers */
#include <stddef.h>        /* size_t */
#include <stdlib.h>       /* abort */
#include <stdbool.h>      /* bool */
#include "../utils/digit_len.h"     /* int digit_len(idx_t); */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t); */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t*, size_t, size_t); */

/* static state for memoised generation */
static seq_t *cache = NULL;      /* cached k values */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t next_k = 1;      /* next integer to test */

/* ensure the cache can accommodate at least len entries */
static void grow_cache(size_t len)
{
    if (len > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < len) new_cap = len;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Return the n-th term (n ≥ 1).  Undefined behavior for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, return 0 */

    /* Expand cache until we have enough terms. */
    while (cache_len < n) {
        uint64_t k = next_k++;                     /* candidate k */
        int d = digit_len((idx_t)k);               /* number of decimal digits of k */

        /* Construct the number 2‖k‖3 = 2 * 10^(d+1) + k * 10 + 3. */
        seq_t p = 2 * pow10((idx_t)(d + 1))          /* 2 * 10^(d+1) */
                  + (seq_t)k * 10                    /* + k * 10 */
                  + 3;                               /* + 3 */

        /* Test primality. */
        if (is_prime_big(p)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
