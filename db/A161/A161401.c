/*
 * A161401  Numbers n such that the count of primes among the permutations of
 *          the digits of n is greater than 1.
 * https://oeis.org/A161401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 4690, u32 n <= 4690, u64 n <= 4690,
 *            u128 n <= 4690, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:
 *   a(n) is the n‑th integer whose decimal‑digit permutations
 *   contain more than one distinct prime.  The implementation
 *   iteratively tests successive integers, storing those that
 *   satisfy the property in a growing cache so that later
 *   calls to a() can be answered in O(1) after the required
 *   terms have already been found.  The test uses the
 *   helper function isPrimeGeneric(seq_t) to decide primality.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

/* Return true if the next lexicographic permutation of arr[0..n-1] exists.
 * The array is modified in place.  The algorithm works correctly for
 * arrays containing duplicate values. */
static bool next_permutation(uint8_t *arr, size_t n)
{
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (arr[j] <= arr[i]) j--;
    uint8_t tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        tmp = arr[l]; arr[l] = arr[r]; arr[r] = tmp;
        l++; r--;
    }
    return true;
}

/* Simple insertion sort for small arrays of digits */
static void sort_digits(uint8_t *d, size_t len)
{
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = d[i];
        size_t j = i;
        while (j > 0 && d[j - 1] > key) {
            d[j] = d[j - 1];
            --j;
        }
        d[j] = key;
    }
}

/* Check whether the decimal number cand has at least two distinct
 * primes among all permutations of its digits.  Leading zeros in a
 * permutation are ignored automatically by the integer construction. */
static bool has_multiple_prime_permutations(idx_t cand)
{
    /* Extract decimal digits into array (least significant first) */
    uint8_t digits[32];
    size_t len = 0;
    idx_t tmp = cand;
    do {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* Sort digits to start with the lexicographically smallest
     * permutation, which allows next_permutation to generate all
     * unique permutations. */
    sort_digits(digits, len);

    /* Store distinct primes found in this number's permutations. */
    seq_t primes[20];
    size_t pcnt = 0;

    /* Iterate over all permutations. */
    do {
        seq_t val = 0;
        for (size_t i = 0; i < len; ++i)
            val = val * 10 + digits[i];

        if (!isPrimeGeneric(val))
            continue;

        /* Avoid counting the same prime twice. */
        bool dup = false;
        for (size_t i = 0; i < pcnt; ++i)
            if (primes[i] == val) { dup = true; break; }
        if (dup)
            continue;

        primes[pcnt++] = val;
        if (pcnt >= 2)
            return true;
    } while (next_permutation(digits, len));

    return false;
}

/* Cache of sequence terms generated so far. */
static seq_t  *cache      = NULL;
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;
static idx_t   last_seen  = 0;   /* Largest integer examined so far */

/* Ensure the cache has enough capacity to append one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();     /* Allocate failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Main sparsely evaluated function.
 *   a(1) = 13, a(2) = 17, …  (see OEIS A161401). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* Out of range (offset 1) */

    while (cache_len < (size_t)n) {
        /* Examine next candidate integer. */
        last_seen++;
        idx_t cand = last_seen;

        if (has_multiple_prime_permutations(cand)) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
