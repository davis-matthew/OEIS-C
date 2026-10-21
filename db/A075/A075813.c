/*
 * A075813  Palindromic even numbers with exactly 2 prime factors (counted
 *          with multiplicity). Equivalently, palindromic numbers of the form
 *          2*p with p prime.
 * https://oeis.org/A075813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th even palindrome having exactly two prime factors
 *            (2 and a prime p). Computed by enumerating palindromes in
 *            increasing order and testing that the number is even and that
 *            (number/2) is prime
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   isPrimeGeneric, seq_alloc, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_free_array.h"

/* Build a palindrome from 'left': if 'odd' is true, the middle digit
   is omitted when mirroring. */
static seq_t build_palindrome(seq_t left, bool odd)
{
    seq_t right = odd ? left / (seq_t)10 : left;
    seq_t result = left;
    while (right > (seq_t)0) {
        seq_t d = right % (seq_t)10;
        result = result * (seq_t)10 + d;
        right /= (seq_t)10;
    }
    return result;
}

/* Generator of palindromes in increasing numeric order.
   Maintains state between calls. */
static seq_t next_palindrome(void)
{
    /* Powers of ten up to 10^19; sufficient for the expected range. */
    static const uint64_t pow10[20] = {
        1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
        1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
        10000000000ULL, 100000000000ULL, 1000000000000ULL,
        10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
        10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL,
        10000000000000000000ULL
    };

    static uint64_t curr_len   = 1;
    static uint64_t half_start = 1, half_end = 9;
    static uint64_t curr_half  = 1;
    static bool curr_odd       = true;

    /* Current palindrome */
    seq_t pal = build_palindrome((seq_t)curr_half, curr_odd);

    /* Advance to the next half for the next call */
    ++curr_half;
    if (curr_half > half_end) {
        /* Increase length of palindromes */
        ++curr_len;
        uint64_t half_len = (curr_len + 1) / 2;
        half_start = (curr_len == 1) ? 1 : pow10[half_len - 1];
        half_end   = pow10[half_len] - 1;
        curr_half  = half_start;
        curr_odd   = (curr_len & 1ULL) != 0;
    }

    return pal;
}

/* ----- Cache for already computed terms ----- */
static seq_t *cache      = NULL;  /* array of found palindromes */
static size_t cache_size = 0;      /* number of elements stored */
static size_t cache_cap   = 0;      /* allocated capacity */

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = seq_alloc(new_cap);
    if (!new_cache) abort();

    /* Copy existing elements */
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];

    /* Free old storage */
    if (cache) seq_free_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function: return the nth term of A075813 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: ignore index 0 */

    size_t desired = (size_t)n;     /* n is uint64_t; fits in size_t for our limits */

    /* Grow cache to at least 'desired' entries */
    ensure_cache(desired);

    /* Generate palindromic candidates until we have enough */
    while (cache_size < desired) {
        seq_t pal = next_palindrome();

        /* Must be even */
        if ((pal % (seq_t)2) != (seq_t)0) continue;

        /* The factor p = pal/2 must be prime */
        seq_t p = pal / (seq_t)2;
        if (!isPrimeGeneric(p)) continue;

        /* Store the valid term */
        cache[cache_size++] = pal;
    }

    return cache[desired - 1];
}
