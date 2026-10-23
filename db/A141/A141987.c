/*
 * A141987  Primes congruent to 11 mod 29.
 * https://oeis.org/A141987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 231, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 11 modulo 29.
 * The function works by sieving all primes up to a safe
 * upper bound (≈ 400*n + 100) and selecting those with
 * the required congruence. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Integer square root using binary search, avoids overflow. */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined below offset, return 0 */

    /* Safe upper bound for the nth term.
       For n up to 10,000 this gives at most ~4,000,100.   */
    idx_t limit = n * 400 + 100;

    /* Allocate sieve array: false = prime, true = composite. */
    bool *isComposite = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!isComposite) return 0;     /* out of memory */

    memset(isComposite, 0, (size_t)(limit + 1) * sizeof(bool));

    /* Eratosthenes sieve */
    idx_t sqrt_limit = int_sqrt(limit);
    for (idx_t p = 2; p <= sqrt_limit; ++p) {
        if (!isComposite[p]) {
            for (idx_t mul = p * p; mul <= limit; mul += p)
                isComposite[mul] = true;
        }
    }

    /* Find the nth prime with p % 29 == 11 */
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!isComposite[i] && (i % 29 == 11)) {
            ++count;
            if (count == n) {
                result = (seq_t)i;
                break;
            }
        }
    }

    free(isComposite);
    return result;
}
