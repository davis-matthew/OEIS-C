/*
 * A052294  Pernicious numbers: numbers with a prime number of 1's in their
 *          binary expansion.
 * https://oeis.org/A052294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nice, nonn
 * Formula:   a(n) = the n-th positive integer whose binary representation *
 *            contains a prime number of 1's
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* Return true iff k is a prime (k <= 200).  The maximum popcount needed
 * for n ≈ 10 000 is far below 200, so this table covers all cases. */
static bool is_prime_small(unsigned k)
{
    if (k < 2) return false;
    if (k == 2) return true;
    if (k % 2 == 0) return false;
    for (unsigned i = 3; i * i <= k; i += 2)
        if (k % i == 0) return false;
    return true;
}

/* Count the number of set bits in a seq_t value.  Works for all supported
 * seq_t types (integer types or the arbitrary‑precision class). */
static unsigned popcnt_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* Ensure that the dynamic array *arr of length *len and capacity *cap
 * can hold at least 'need' elements, allocating or reallocating as
 * necessary.  Works for both C and C++ builds. */
static void ensure_capacity(seq_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (*cap >= need) return;

    size_t new_cap = (*cap == 0 ? 64 : (*cap < 64 ? 64 : *cap * 2));
    if (new_cap < need) new_cap = need;

    seq_t *new_arr = NULL;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_arr) exit(1);

    for (size_t i = 0; i < *len; ++i)
        new_arr[i] = (*arr)[i];

#ifdef __cplusplus
    delete [] *arr;
#else
    free(*arr);
#endif
    *arr = new_arr;
    *cap = new_cap;
}

/* Static cache of computed pernicious numbers. */
static seq_t *seq_cache = NULL;   /* array of seq_t */
static size_t   seq_len   = 0;    /* number of cached entries */
static size_t   seq_cap   = 0;    /* allocated capacity */
static uint64_t last_num   = 2;   /* number that has been tested last */

/* Return the n‑th pernicious number (n ≥ 1).  The function is safe for
 * any n ≥ 1 and works with any build of seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* guard against accidental zero */

    while (seq_len < (size_t)n) {
        ++last_num;
        seq_t candidate = (seq_t)last_num;
        unsigned pc = popcnt_seq(candidate);

        if (is_prime_small(pc)) {
            ensure_capacity(&seq_cache, &seq_len, &seq_cap, seq_len + 1);
            seq_cache[seq_len++] = candidate;
        }
    }
    return seq_cache[n - 1];
}
