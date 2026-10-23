/*
 * A032717  Numbers k such that k prefixed by '9' and followed by '3' is
 *          prime.
 * https://oeis.org/A032717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) returns the n‑th integer k such that the number 9 k 3 * (9
 *            concatenated with k followed by 3) is prime. No simpler * closed
 *            form is known. The function memoizes found terms so that * calls
 *            are O(1) after the term has been discovered. All arithmetic * is
 *            performed in the type seq_t, which may...
 * Range:     u8 none, u16 n <= 26, u32 n <= 26, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digits_of.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static idx_t last_checked = 0;   /* largest k examined so far      */
    static seq_t *cache = NULL;      /* array of discovered k values   */
    static size_t cache_len = 0;     /* number of terms stored         */
    static size_t cache_cap = 0;     /* current capacity of cache      */
    static seq_t pow_cache = 0;      /* 10^(len(k)+1) for current k    */
    static int  pow_digits = -1;     /* len(k)+1 that pow_cache holds  */

    if (n == 0) return 0;           /* offset is 1, undefined below */

    if (n <= cache_len)             /* value already memoized */
        return cache[n - 1];

    if (cache == NULL) {            /* allocate initial cache      */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    for (idx_t k = last_checked + 1; ; ++k) {
        int len = digits_of(k);       /* number of decimal digits of k */
        int target = len + 1;         /* exponent for 10^(len+1)     */
        if (target != pow_digits) {
            pow_cache = pow10((idx_t)target);
            pow_digits = target;
        }
        seq_t num = pow_cache * 9 + (seq_t)k * 10 + 3;  /* 9k3 */
        if (is_prime_big(num)) {              /* prime test */
            if (cache_len >= cache_cap) {     /* grow cache if needed */
                size_t new_cap = cache_cap * 2;
                if (new_cap < cache_len + 1)
                    new_cap = cache_len + 1;
                seq_t *newptr = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
                if (!newptr) abort();
                cache    = newptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
            if (cache_len >= n) {
                last_checked = k;
                return cache[n - 1];
            }
        }
        last_checked = k;   /* advance to next candidate             */
    }
}
