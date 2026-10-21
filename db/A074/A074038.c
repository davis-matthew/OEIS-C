/*
 * A074038  If n is the k-th single (i.e., non-twin) prime then a(n) = k,
 *          otherwise a(n) = 0.
 * https://oeis.org/A074038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the 1‑based index of n among the non‑twin primes; for
 *            non‑prime or twin‑prime values a(n)=0. Computed by a sieve that
 *            is expanded as needed
 * Range:     u8 n <= 2856, u16 n <= 9123, u32 n <= 9123, u64 n <= 9123,
 *            u128 n <= 9123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static size_t sieve_limit  = 0;
static uint8_t *is_prime_arr = NULL;
static size_t *single_prefix = NULL;

/* Expand the sieve so that all numbers up to new_limit are represented
   and the prefix array of single primes is recomputed. */
static void extend_sieve(idx_t new_limit)
{
    if ((idx_t)sieve_limit >= new_limit)
        return;

    size_t nl = (size_t)new_limit;
    uint8_t *new_is = (uint8_t *)calloc(nl + 1, sizeof(uint8_t));
    if (!new_is)
        return;                     /* allocation failure – leave empty */
    for (size_t i = 2; i <= nl; ++i)
        new_is[i] = 1;

    for (size_t i = 2; (uint64_t)i * i <= nl; ++i) {
        if (!new_is[i])
            continue;
        for (size_t j = i * i; j <= nl; j += i)
            new_is[j] = 0;
    }

    size_t *new_pref = (size_t *)malloc((nl + 1) * sizeof(size_t));
    if (!new_pref) {
        free(new_is);
        return;
    }
    new_pref[0] = 0;
    for (size_t i = 1; i <= nl; ++i) {
        size_t inc = 0;
        if (new_is[i]) {
            bool twin = false;
            if (i >= 2 && new_is[i - 2])
                twin = true;
            if (i + 2 <= nl && new_is[i + 2])
                twin = true;
            if (!twin)
                inc = 1;
        }
        new_pref[i] = new_pref[i - 1] + inc;
    }

    free(is_prime_arr);
    free(single_prefix);
    is_prime_arr = new_is;
    single_prefix = new_pref;
    sieve_limit = nl;
}

seq_t a(idx_t n)
{
    if (n < 2)          /* 0 and 1 are not prime */
        return 0;

    /* Ensure the sieve covers n+2 to test for twins */
    if ((idx_t)sieve_limit < n + 2)
        extend_sieve(n + 2);

    size_t idx = (size_t)n;
    if (!is_prime_arr[idx])
        return 0;

    bool twin = false;
    if (idx >= 2 && is_prime_arr[idx - 2])
        twin = true;
    if (idx + 2 <= sieve_limit && is_prime_arr[idx + 2])
        twin = true;
    if (twin)
        return 0;

    return (seq_t)single_prefix[idx];
}
