/*
 * A157297  Positive numbers y such that y^2 is of the form x^2+(x+233)^2 with
 *          integer x.
 * https://oeis.org/A157297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6 Initial values: a(1)=185,
 *            a(2)=233, a(3)=317, a(4)=793, a(5)=1165, a(6)=1717
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 68, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *seq_cache = NULL;
static idx_t seq_cache_max = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    if (n > seq_cache_max) {
        idx_t old_max = seq_cache_max;
        /* Allocate space for indices 1..n; index 0 unused */
        seq_t *tmp = (seq_t*)realloc(seq_cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();                       /* allocation failed */
        seq_cache = tmp;
        seq_cache_max = n;

        for (idx_t k = old_max + 1; k <= n; ++k) {
            switch (k) {
            case 1: seq_cache[1] = 185; break;
            case 2: seq_cache[2] = 233; break;
            case 3: seq_cache[3] = 317; break;
            case 4: seq_cache[4] = 793; break;
            case 5: seq_cache[5] = 1165; break;
            case 6: seq_cache[6] = 1717; break;
            default:
                /* recurrence: a(k) = 6*a(k-3) - a(k-6) */
                seq_cache[k] = 6 * seq_cache[k-3] - seq_cache[k-6];
                break;
            }
        }
    }

    return seq_cache[n];
}
