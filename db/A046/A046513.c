/*
 * A046513  Numbers with multiplicative persistence value 4.
 * https://oeis.org/A046513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5805, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046513: numbers with multiplicative persistence 4.
   The function returns the n‑th such positive integer (n ≥ 1). */
#include <stdbool.h>
#include <stddef.h>

#define MAX_CACHE 20000    /* should be > expected number of terms (e.g. 10 000) */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t next_candidate = 1;

seq_t a(idx_t n)
{
    /* Fill cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cur = next_candidate++;
        /* Skip numbers with a zero digit: such numbers have persistence ≤ 1. */
        seq_t tmp = cur;
        bool has_zero = false;
        while (tmp > 0) {
            if ((tmp % 10) == 0) {
                has_zero = true;
                break;
            }
            tmp /= 10;
        }
        if (has_zero)
            continue;

        /* Compute multiplicative persistence of cur. */
        idx_t steps = 0;
        seq_t x = cur;
        while (x > 9 && steps <= 4) {
            seq_t prod = 1;
            seq_t y = x;
            while (y > 0) {
                prod *= (y % 10);
                y /= 10;
            }
            x = prod;
            steps++;
        }

        if (steps == 4) {
            cache[cache_len++] = cur;
        }
    }
    return cache[(size_t)(n - 1)];
}
