/*
 * A208086  Number of 4 X (n+1) 0..1 arrays with every 2 X 2 subblock having
 *          the same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A208086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=24, a(2)=56, a(3)=134, a(4)=344 a(n) = 3*a(n-1) - 3*a(n-3)
 *            + a(n-4) for n ≥ 5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static cache whose size covers
   the expected maximum n (here up to 20000).  The cache
   grows lazily and is re‑used across calls. */

#include <stddef.h>
#include <stdbool.h>

static seq_t cache[20001];          /* 1‑based indexing; 0 unused */
static idx_t computed_until = 0;   /* largest index that has a valid value */

/* Return the nth term of A208086, 1‑based (a(1) = 24). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;         /* not part of the sequence */

    /* compute terms up to n if necessary */
    if (n > computed_until) {
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            if (i == 1)
                cache[1] = (seq_t)24;
            else if (i == 2)
                cache[2] = (seq_t)56;
            else if (i == 3)
                cache[3] = (seq_t)134;
            else if (i == 4)
                cache[4] = (seq_t)344;
            else
                cache[i] = 3 * cache[i-1] - 3 * cache[i-3] + cache[i-4];
        }
        computed_until = n;
    }

    return cache[n];
}
