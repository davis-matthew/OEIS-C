/*
 * A090264  Numbers n such that the strings n7n and 7n7 are both prime.
 * https://oeis.org/A090264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th integer n such that the decimal strings n7n and
 *            7n7 are both prime (OEIS A090264). The function implements a
 *            linear search with memoisation, testing each integer in turn
 *            until the required number of terms have been found
 * Range:     u8 none, u16 none, u32 n <= 1460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

static bool is_candidate(idx_t x)
{
    /* Static state keeps track of the current number of decimal digits
       and the corresponding powers of ten.  They are updated only when
       the candidate reaches the next digit threshold. */
    static int digits = 1;            /* current number of digits of x */
    static seq_t pow10d  = 10;        /* 10^digits   */
    static seq_t pow10d1 = 100;       /* 10^(digits+1) */
    static idx_t next_pow10 = 10;     /* the next 10^digits threshold */

    if (x == next_pow10) {
        /* digit count increases */
        digits++;
        pow10d  = pow10d  * (seq_t)10;
        pow10d1 = pow10d1 * (seq_t)10;
        next_pow10 *= 10;
    }

    seq_t n   = (seq_t)x;
    seq_t n7n = n * pow10d1 + ((seq_t)7 * pow10d) + n;
    seq_t s_n7 = ((seq_t)7 * pow10d1) + (n * (seq_t)10) + ((seq_t)7);

    return isPrimeGeneric(n7n) && isPrimeGeneric(s_n7);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0 */

    /* Lazy dynamic cache of discovered terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static idx_t candidate   = 1;  /* next integer to test */

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_size < n) {
        if (is_candidate(candidate)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate++;
    }

    return cache[n - 1];  /* offset is 1 */
}
