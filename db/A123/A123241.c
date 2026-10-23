/*
 * A123241  Number of squares < lesser of twin primes.
 * https://oeis.org/A123241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(A001359(n))) + 1 where A001359(n) is the n-th
 *            lower twin prime. This counts the number of perfect squares less
 *            than (or equal to) the lower twin prime, then adds one
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root for 64‑bit unsigned numbers */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Simple deterministic primality test using trial division */
static bool is_prime_simple(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    idx_t r = floor_isqrt_u64(n);
    for (idx_t d = 3; d <= r; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Expand a dynamic array of idx_t values */
static void ensure_idx_cap(size_t needed, size_t *cap, idx_t **ptr)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = (idx_t *)realloc(*ptr, new_cap * sizeof(idx_t));
        *cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n=0 is undefined */

    static idx_t *twin = NULL;
    static size_t twin_count = 0;
    static size_t twin_capacity = 0;
    static idx_t next_candidate = 3;   /* start scanning from 3 */

    /* Generate twin lower primes until we have at least n of them */
    while (twin_count < n) {
        /* Find next twin pair */
        while (!(is_prime_simple(next_candidate) &&
                 is_prime_simple(next_candidate + 2))) {
            next_candidate += 2;
        }

        /* Store the lower member of the twin pair */
        ensure_idx_cap(twin_count + 1, &twin_capacity, &twin);
        twin[twin_count++] = next_candidate;

        /* Advance to next odd number */
        next_candidate += 2;
    }

    /* a(n) = floor(sqrt(twin[n-1])) + 1 */
    idx_t lower = twin[n - 1];
    idx_t sqrt_low = floor_isqrt_u64(lower);
    seq_t result = (seq_t)sqrt_low + (seq_t)1;
    return result;
}
