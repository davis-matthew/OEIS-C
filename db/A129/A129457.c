/*
 * A129457  a(1)=1; a(n) = number of earlier terms of the sequence that are
 *          coprime to (n+a(n-1)).
 * https://oeis.org/A129457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1
   a(n) = number of terms a(j) (1 ≤ j < n) that are coprime to (n + a(n‑1)). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* dynamic array of a(n) */
static idx_t  maxComputed = 0;       /* largest n for which a(n) is stored */
static size_t capacity   = 0;        /* current buffer size */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (capacity >= needed) return;
    size_t new_cap = capacity ? capacity * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, return 0 for safety */

    /* Initialise the first term on first call. */
    if (maxComputed == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;       /* a(1) = 1 */
        maxComputed = 1;
    }

    /* If the requested term is already known, return it. */
    if (n <= maxComputed) return cache[n-1];

    /* Extend the cache up to n. */
    ensure_capacity(n);
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t M = (seq_t)i + cache[i-2];   /* i ≥ 2 so i-2 ≥ 0 */
        seq_t cnt = 0;
        for (idx_t j = 0; j < i-1; ++j) {
            if (gcd(cache[j], M) == 1) ++cnt;
        }
        cache[i-1] = cnt;
    }
    maxComputed = n;
    return cache[n-1];
}
