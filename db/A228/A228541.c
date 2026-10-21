/*
 * A228541  Numbers having at least one prime factor of the form 30*k + 1.
 * https://oeis.org/A228541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 9303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228541: numbers that have at least one prime divisor of the form 30*k + 1 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Find the smallest prime factor of n (trial division, 6k±1 wheel) */
static uint64_t smallest_factor(uint64_t n)
{
    if (n < 2) return n;
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t d = 5; d * d <= n; d += 6)
    {
        if (n % d == 0) return d;
        if (n % (d + 2) == 0) return d + 2;
    }
    return n;          /* n itself is prime */
}

/* Check whether n has a prime divisor congruent to 1 modulo 30 */
static bool has_1mod30_prime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t m = n;
    while (m > 1)
    {
        uint64_t p = smallest_factor(m);
        if (p % 30 == 1) return true;
        while (m % p == 0) m /= p;
    }
    return false;
}

/* Sequence builder / accessor */
seq_t a(idx_t n)
{
    /* Off‑by‑one: a(1) is the first element of the table */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t max_scanned = 0;

    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* Grow the precomputed table until it contains n elements */
    if (seq_len < (size_t)n)
    {
        if (seq == NULL)
        {
            seq_cap = 64;
            seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        }

        /* Choose a scanning limit and enlarge it as needed */
        idx_t limit = max_scanned ? max_scanned * 2 : 1000;
        while (seq_len < (size_t)n)
        {
            if (max_scanned >= limit)
            {
                limit <<= 1;           /* double the limit */
                continue;
            }
            ++max_scanned;            /* next integer to test */

            if (has_1mod30_prime((uint64_t)max_scanned))
            {
                if (seq_len == seq_cap)
                {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)max_scanned;
            }
        }
    }

    return seq[n - 1];
}
