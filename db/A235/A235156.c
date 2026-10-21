/*
 * A235156  Primes which have one or more occurrences of exactly four
 *          different digits.
 * https://oeis.org/A235156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with exactly four distinct decimal digits. */

/* We use the provided utilities for digit counting and primality testing. */
#include "../utils/digit_counts.h"
#include "../utils/is_prime_big.h"
#include <stdlib.h>

/* Dynamically enlarge an array of seq_t if needed. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    static seq_t *cached    = NULL;            /* cached sequence */
    static size_t cachedlen = 0, cachedcap = 0;
    static seq_t current = 2;                  /* next candidate to test */

    while (cachedlen < (size_t)n) {
        /* Test primality */
        if (is_prime_big(current)) {
            unsigned char cnt[10] = {0};
            digit_counts(current, cnt);
            int distinct = 0;
            for (int d = 0; d < 10; ++d) if (cnt[d]) ++distinct;
            if (distinct == 4) {
                ensure_capacity(&cached, &cachedcap, cachedlen + 1);
                cached[cachedlen++] = current;
            }
        }
        ++current;
    }
    return cached[(size_t)n - 1];
}
