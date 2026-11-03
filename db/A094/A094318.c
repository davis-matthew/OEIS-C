/*
 * A094318  Primes n with property that sum of digits that are 1 or a prime is
 *          prime.
 * https://oeis.org/A094318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes n such that the sum of digits that are 1 or a prime
 *            (2,3,5,7) is itself a prime. The function returns the n‑th
 *            element of this sequence (a(1) = 2)
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime, is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* helper: determine whether a decimal digit is 1 or a prime digit */
static inline bool is_special_digit(uint64_t d)
{
    return d == 1ULL || d == 2ULL || d == 3ULL || d == 5ULL || d == 7ULL;
}

static seq_t *filtered = NULL;   /* array of qualifying primes          */
static size_t filtered_len = 0;  /* number of stored primes             */
static size_t filtered_cap = 0;  /* current capacity of the array       */
static uint64_t candidate = 2;   /* next integer to test for primality */

/* ensure capacity for at least 'need' elements */
static void ensure_filtered_cap(size_t need)
{
    if (need <= filtered_cap)
        return;
    size_t new_cap = filtered_cap ? filtered_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *newp = mem_resize(seq_t, filtered, new_cap);
    filtered = newp;
    filtered_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* expand the list until we have at least n qualifying primes */
    while (filtered_len < (size_t)n)
    {
        /* find next prime candidate */
        while (!is_prime((idx_t)candidate))
            ++candidate;

        /* compute sum of digits that are 1 or prime digits */
        uint64_t sum = 0;
        uint64_t tmp = candidate;
        while (tmp > 0)
        {
            uint64_t d = tmp % 10ULL;
            if (is_special_digit(d))
                sum += d;
            tmp /= 10ULL;
        }

        /* if the sum itself is prime, store the candidate */
        if (is_prime_seq((seq_t)sum))
        {
            ensure_filtered_cap(filtered_len + 1);
            filtered[filtered_len++] = (seq_t)candidate;
        }
        ++candidate;   /* next integer to test */
    }

    return filtered[(size_t)n - 1];
}
