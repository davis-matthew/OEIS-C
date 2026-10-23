/*
 * A106758  Primes with digit sum = 17.
 * https://oeis.org/A106758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose decimal digit sum = 17
 * Range:     u8 n <= 3, u16 n <= 518, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   digit_sum_uint64, is_prime_uint64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/digit_sum_uint64.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Handle the OEIS offset: a(1) is the first term. */
    if (n == 0) return 0; /* undefined for n < 1 */

    /* Static storage for memoization. */
    static seq_t *primes = NULL;
    static size_t size  = 0;   /* number of primes found so far */
    static size_t cap   = 0;   /* allocated capacity */
    static uint64_t candidate = 2; /* next integer to test */

    /* Generate primes with digit sum 17 until we have at least n of them. */
    while ((idx_t)size < n)
    {
        /* Find the next qualifying prime. */
        for (;; candidate++)
        {
            if (is_prime_uint64(candidate) &&
                digit_sum_uint64(candidate) == (seq_t)17)
                break;
        }

        /* Resize the array if necessary. */
        if (size == cap)
        {
            cap = (cap == 0) ? 16 : cap * 2;
            primes = (seq_t *)seq_realloc(primes, cap);
        }

        primes[size++] = (seq_t)candidate;
        candidate++; /* start next check from the following integer */
    }

    return primes[n - 1];
}
