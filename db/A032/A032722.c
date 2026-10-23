/*
 * A032722  Numbers k such that k prefixed by '5' and followed by '7' is
 *          prime.
 * https://oeis.org/A032722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032722: numbers k such that 5·10^(digits(k)+1)+k·10+7 is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache and generation state */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    static idx_t next_k     = 0;   /* smallest k not yet examined */
    static idx_t transition = 10;  /* first k where the digit count increases */
    static seq_t pow10_d    = 10;  /* 10^d, starting with d = 1 */

    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n collected */
    while (cache_size < n) {
        /* update digit count when crossing a power of ten */
        if (next_k >= transition) {
            pow10_d *= 10;          /* now 10^(d+1) */
            transition *= 10;       /* next threshold */
        }

        /* construct the candidate number */
        seq_t number = 5 * (pow10_d * 10) + (seq_t)next_k * 10 + 7;

        if (is_prime_big(number)) {
            /* ensure capacity for the new k */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
