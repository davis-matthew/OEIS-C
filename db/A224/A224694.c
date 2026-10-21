/*
 * A224694  Numbers n such that n^2 AND n = 0, where AND is the bitwise
 *          logical AND operator.
 * https://oeis.org/A224694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 2037, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224694: numbers n such that (n*n) & n == 0 */

#include <stdint.h>
#include <stddef.h>

#define CACHE_CAP 200000

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t current = 1;

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
        current = 1; /* start scanning from 1 */
    }

    while (cache_len < n) {
        seq_t cand;

        /* Find the next candidate satisfying (cand*cand) & cand == 0 */
        while (1) {
            cand = (seq_t)current;
            seq_t sq = cand * cand;
            if ((sq & cand) == 0) {
                break;
            }
            current++;
        }

        cache[cache_len++] = cand;
        current++; /* next candidate for future scans */
    }

    return cache[n - 1];
}
