/*
 * A215304  Primes congruent to {1, 2, 3, 4} mod 17.
 * https://oeis.org/A215304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p mod 17 ∈ {1,2,3,4} (OEIS A215304)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* initial sieving limit – enough for n ≈ 10000 (≈4×10000‑th prime ≈ 400 000) */
static const idx_t INITIAL_LIMIT = 700000;
static bool sieve_initialized = false;
static seq_t *filtered_primes = NULL;
static size_t filtered_count = 0;

/* Build a sieve up to 'limit' and collect the primes ≡ 1,2,3,4 (mod 17). */
static void build_sieve(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    /* use C++‑compatible casts */
    char *is_composite = (char*)calloc(sz, sizeof(char));
    if (!is_composite) return;

    for (size_t i = 2; i * i < sz; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j < sz; j += i)
                is_composite[j] = 1;

    /* count how many qualifying primes we will keep */
    filtered_count = 0;
    for (size_t i = 2; i < sz; ++i)
        if (!is_composite[i]) {
            uint64_t rem = i % 17;
            if (rem == 1 || rem == 2 || rem == 3 || rem == 4)
                ++filtered_count;
        }

    filtered_primes = (seq_t*)realloc(filtered_primes,
                                      filtered_count * sizeof(seq_t));
    if (!filtered_primes) {
        free(is_composite);
        return;
    }

    /* fill the array with the qualifying primes */
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (!is_composite[i]) {
            uint64_t rem = i % 17;
            if (rem == 1 || rem == 2 || rem == 3 || rem == 4)
                filtered_primes[idx++] = (seq_t)i;
        }

    free(is_composite);
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */

    /* initialise sieve lazily */
    if (!sieve_initialized) {
        build_sieve(INITIAL_LIMIT);
        sieve_initialized = true;
    }

    /* If we don't have enough primes, grow the sieve limit. */
    if ((idx_t)filtered_count < n) {
        idx_t new_limit = INITIAL_LIMIT * 2;
        while ((idx_t)filtered_count < n) {
            free(filtered_primes);
            filtered_primes = NULL;
            filtered_count = 0;
            build_sieve(new_limit);
            if ((idx_t)filtered_count >= n) break;
            new_limit *= 2;
        }
    }

    return filtered_primes[(size_t)(n - 1)];
}
