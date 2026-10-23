/*
 * A160677  Numbers k such that k, 2k and 3k use the same number of characters
 *          when expressed in Roman numerals.
 * https://oeis.org/A160677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A160677: numbers k such that k, 2 k and 3 k
 * have the same number of characters when written as Roman numerals.
 * This implementation performs an incremental linear search and caches
 * the found values for future calls.  It is compatible with the
 * standard built‑in integer types as well as an arbitrary‑precision
 * seq_t type (the cast of realloc is required for C++). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Lengths of Roman numeral representations for the digits 0‑9.
   The digit “0” is unused but kept for easier indexing. */
static const unsigned char roman_digit_len[10] = {
    0, /* 0 */
    1, /* I      */
    2, /* II     */
    3, /* III    */
    2, /* IV     */
    1, /* V      */
    2, /* VI     */
    3, /* VII    */
    4, /* VIII   */
    2  /* IX     */
};

/* Returns the number of characters required to write `num` in Roman numerals. */
static size_t roman_len(idx_t num)
{
    size_t len = 0;

    /* Thousands: repeated 'M' */
    size_t thousands = num / 1000U;
    len += thousands;
    num %= 1000U;

    /* Hundreds */
    size_t hundreds = num / 100U;
    len += roman_digit_len[hundreds];
    num %= 100U;

    /* Tens */
    size_t tens = num / 10U;
    len += roman_digit_len[tens];

    /* Ones */
    size_t ones = num % 10U;
    len += roman_digit_len[ones];

    return len;
}

/* Cached sequence data. */
static seq_t *cache = NULL;          /* array of values */
static size_t cache_size = 0;       /* number of values stored */
static size_t cache_cap  = 0;       /* allocated capacity */
static idx_t next_check = 1;        /* next integer to test */

/* Ensure that `cache` has capacity for at least `required` entries. */
static void ensure_cache_capacity(size_t required)
{
    if (required <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < required)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);           /* out of memory */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Append one value to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = val;
}

/* Return the n-th term of A160677 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* guard against zero input */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        idx_t k = next_check++;

        size_t len_k  = roman_len(k);
        size_t len_2k = roman_len(k + k);
        size_t len_3k = roman_len(k + k + k);

        if (len_k == len_2k && len_2k == len_3k)
            cache_append((seq_t)k);
    }

    return cache[(size_t)n - 1];
}
