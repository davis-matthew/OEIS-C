/*
 * A163967  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -136*a(n-6) + 16*∑_{k=1..5} a(n-k) for n ≥ 7, a(0)=1,
 *            a(1)=18, a(2)=306, a(3)=5202, a(4)=88434, a(5)=1503378,
 *            a(6)=25557273. The recurrence for n=6 has an extra +1 term
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_size = 0;      /* number of elements currently stored */

    /* Ensure we have at least 6 entries (base values). */
    size_t required = (size_t)n + 1;
    if (required < 6)
        required = 6;

    if (cache == NULL) {
        cache = (seq_t *)malloc(required * sizeof(seq_t));
    } else if (required > cached_size) {
        cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    }

    /* Fill missing values. */
    for (idx_t i = (idx_t)cached_size; i < (idx_t)required; ++i) {
        if (i < 6) {                /* initialise base cases */
            switch (i) {
                case 0: cache[i] = (seq_t)1;       break;
                case 1: cache[i] = (seq_t)18;      break;
                case 2: cache[i] = (seq_t)306;     break;
                case 3: cache[i] = (seq_t)5202;    break;
                case 4: cache[i] = (seq_t)88434;   break;
                case 5: cache[i] = (seq_t)1503378; break;
            }
        } else if (i == 6) {        /* a(6) has an extra +1 term */
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
            cache[i] = sum * (seq_t)16 - (seq_t)136 * cache[i-6] + (seq_t)1;
        } else {                    /* recurrence for n≥7 */
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
            cache[i] = sum * (seq_t)16 - (seq_t)136 * cache[i-6];
        }
    }
    cached_size = required;

    return cache[(size_t)n];
}
