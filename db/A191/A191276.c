/*
 * A191276  Numbers that are congruent to {0, 1, 4, 5, 7, 9, 11} mod 12.
 * https://oeis.org/A191276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-7) - a(n-8) for n>8;
   initial values: a(1)=0, a(2)=1, a(3)=4, a(4)=5, a(5)=7, a(6)=8,
   a(7)=11, a(8)=12 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0; /* Highest index that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* Undefined for n<1, guard against underflow */

    if (seq_cache == NULL) {
        /* Allocate storage for indices 1..8 (index 0 unused) */
        seq_cache_size = 8;
        seq_cache = (seq_t *)malloc((seq_cache_size + 1) * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[1] = (seq_t)0;
        seq_cache[2] = (seq_t)1;
        seq_cache[3] = (seq_t)4;
        seq_cache[4] = (seq_t)5;
        seq_cache[5] = (seq_t)7;
        seq_cache[6] = (seq_t)8;
        seq_cache[7] = (seq_t)11;
        seq_cache[8] = (seq_t)12;
    }

    if (n > seq_cache_size) {
        idx_t new_size = n;
        seq_cache = (seq_t *)realloc(seq_cache, (new_size + 1) * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        for (idx_t i = seq_cache_size + 1; i <= new_size; ++i) {
            if (i <= 8) {
                /* already initialized */
                continue;
            }
            seq_cache[i] = seq_cache[i - 1] + seq_cache[i - 7] - seq_cache[i - 8];
        }
        seq_cache_size = new_size;
    }
    return seq_cache[n];
}
