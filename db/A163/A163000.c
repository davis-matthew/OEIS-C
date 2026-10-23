/*
 * A163000  Count of integers x in [0,n] satisfying A000120(x) + A000120(n-x)
 *          = A000120(n) + 1.
 * https://oeis.org/A163000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ x ∈ [0,n] : popcount(x)+popcount(n-x) = popcount(n)+1
 *            }
 * Range:     u8 n <= 702, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function precomputes the sequence up to the argument on demand
   using a simple double‑loop over the current range.  For n ≤ 10000
   this runs in far less than 2 s.  The implementation is fully
   compatible with C11 and C++17. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static caches for computed values and popcounts */
    static seq_t *values = NULL;          /* a[0..m] */
    static unsigned char *popcnt = NULL;   /* popcnt[0..m] */
    static idx_t m = 0;                   /* currently cached up to m-1 */

    /* extend caches if needed */
    if (n >= m) {
        idx_t new_m = n + 1;
        /* reallocate values array */
        seq_t *new_values = (seq_t *)realloc(values, new_m * sizeof(seq_t));
        if (!new_values) return 0;         /* out of memory, return 0 */
        values = new_values;

        /* reallocate popcnt array */
        unsigned char *new_popcnt = (unsigned char *)realloc(popcnt, new_m * sizeof(unsigned char));
        if (!new_popcnt) return 0;
        popcnt = new_popcnt;

        /* compute popcounts for new indices */
        for (idx_t i = m; i < new_m; ++i) {
            /* using gcc/clang builtin popcount for 64-bit integers */
            popcnt[i] = (unsigned char)__builtin_popcountll((unsigned long long)i);
        }

        /* compute a[i] for newly added indices */
        for (idx_t i = m; i < new_m; ++i) {
            unsigned char target = popcnt[i] + 1;
            unsigned int count = 0;
            for (idx_t x = 0; x <= i; ++x) {
                if (popcnt[x] + popcnt[i - x] == target) {
                    ++count;
                }
            }
            values[i] = (seq_t)count;
        }
        m = new_m;
    }

    return values[n];
}
