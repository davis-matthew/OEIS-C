/*
 * A099104  Characteristic function of badly sieved numbers (A066680).
 * https://oeis.org/A099104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod(if n>d^2 then 1 else 1-a(d) : d<n and n mod d = 0);
 *            a(1)=0
 * Range:     u8 n <= 33, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached values: cache[n] holds a(n) after first computation, known[n] indicates it has been computed. */
static seq_t *cache = NULL;
static bool   *known = NULL;
static size_t  cached_size = 0;

/* Ensure that cache arrays cover indices up to n. */
static void ensure_cache(idx_t n)
{
    size_t needed = n + 1;
    if (needed <= cached_size)
        return;

    size_t new_size = cached_size ?: 1;
    while (new_size < needed)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    bool  *new_known = (bool  *)realloc(known, new_size * sizeof(bool));
    if (!new_cache || !new_known)
        exit(1);   /* Allocation failure – terminate promptly */

    cache = new_cache;
    known = new_known;
    for (size_t i = cached_size; i < new_size; ++i) {
        known[i] = false;
        cache[i] = 0;
    }
    cached_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    ensure_cache(n);
    if (known[n])
        return cache[n];

    seq_t result = 1;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        idx_t d1 = i;
        if (d1 < n) {
            seq_t d1sq = (seq_t)d1 * (seq_t)d1;
            if (d1sq >= (seq_t)n) {
                seq_t val = a(d1);
                if (val) {          /* (1 - a(d1)) == 0 */
                    result = 0;
                    break;
                }
            }
        }

        idx_t d2 = n / i;
        if (d2 != d1 && d2 < n) {
            seq_t d2sq = (seq_t)d2 * (seq_t)d2;
            if (d2sq >= (seq_t)n) {
                seq_t val = a(d2);
                if (val) {          /* (1 - a(d2)) == 0 */
                    result = 0;
                    break;
                }
            }
        }
    }

    cache[n] = result;
    known[n] = true;
    return result;
}
