/*
 * A218977  Primes of the form 2n^2 - 181 for n >= 0.
 * https://oeis.org/A218977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*m^2 - 181, where m = 0,1,2... and a(n) is the nth prime
 *            (including negative primes) of this form
 * Range:     i8 none, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"     /* provides void grow_seq(seq_t **arr, size_t *cap, size_t need); */
#include "../utils/is_prime_big.h" /* provides bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /*  Cache of sequence values */
    static seq_t *cache   = NULL;   /* stored values a(1)...a(count) */
    static size_t cap     = 0;      /* current cache capacity */
    static idx_t  count   = 0;      /* number of primes stored so far */
    static idx_t  next_m  = 0;      /* next m to evaluate */

    if (n == 0)                    /* offset starts at 1 */
        return (seq_t)0;

    /* Return cached value if available */
    if (count >= n)
        return cache[n - 1];

    /* Generate terms until we have at least n primes */
    while (count < n) {
        seq_t m   = (seq_t)next_m;
        seq_t val = 2 * (m * m) - (seq_t)181;   /* 2*m^2 - 181 */

        seq_t abs_val = val;
        if (abs_val < 0)
            abs_val = -abs_val;

        /* Test primality of the absolute value */
        if (abs_val >= 2 && is_prime_big(abs_val)) {
            /* Ensure the cache has enough space */
            grow_seq(&cache, &cap, count + 1);

            /* Store the signed result (even if negative) */
            cache[count++] = val;
        }

        /* Move to the next m */
        ++next_m;
    }

    return cache[n - 1];
}
