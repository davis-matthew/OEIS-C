/*
 * A084366  Numbers k such that the k-th prime number has an odd number of
 *          digits and becomes composite if the central digit is removed.
 * https://oeis.org/A084366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_count_seq_t, get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A084366: k such that the k-th prime has an odd number of digits and
 * becomes composite if the central digit is removed.
 * The function builds the sequence lazily and memoises results.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/get_prime.h"      /* get_prime(idx_t k) -> seq_t */
#include "../utils/isPrimeGeneric.h" /* isPrimeGeneric(seq_t n) -> bool */
#include "../utils/digit_count_seq_t.h" /* digit_count_seq_t(seq_t v) -> idx_t */

/* 
 * Compute the nth term of the sequence (offset 1).
 * The implementation uses a lazy search: primes are generated one by one
 * with get_prime() and tested until the required number of terms has been
 * found.  Results are cached so subsequent calls are fast.
 */
seq_t a(idx_t n)
{
    /* static state for lazy computation and memoisation */
    static idx_t test_idx   = 0;          /* last prime index examined */
    static idx_t found_cnt  = 0;          /* how many terms have been found */
    static seq_t *cache    = NULL;        /* array of found k values */
    static size_t cap      = 0;           /* cache capacity */
    static size_t len      = 0;           /* current number of cached terms */

    /* ensure cache is large enough */
    if (len + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* if the requested term is already cached, return it */
    if (n <= len)
        return cache[n - 1];

    /* search for more terms until we reach the nth one */
    while (found_cnt < n) {
        idx_t prime_idx = ++test_idx;          /* next prime index */
        seq_t prime = get_prime(prime_idx);    /* k-th prime */

        /* number of decimal digits of the prime */
        idx_t digits = digit_count_seq_t(prime);
        if (digits & 1) {                      /* odd number of digits? */
            idx_t half = digits / 2;            /* position of central digit */

            /* compute 10^half and 10^(half+1) */
            seq_t pow10_half = 1;
            seq_t pow10_half_plus = 1;
            for (idx_t i = 0; i < half; ++i) {
                pow10_half *= 10;
            }
            pow10_half_plus = pow10_half * 10;  /* 10^(half+1) */

            /* high part (digits before central), low part (after) */
            seq_t high = prime / pow10_half_plus;
            seq_t low  = prime % pow10_half;
            seq_t removed = high * pow10_half + low;

            /* composite after removal? */
            if (removed > (seq_t)1 && !isPrimeGeneric(removed)) {
                /* store the prime index (k) */
                cache[found_cnt] = prime_idx;
                ++found_cnt;
                ++len;   /* cache size grows with each new term */
            }
        }
    }

    return cache[n - 1];
}
