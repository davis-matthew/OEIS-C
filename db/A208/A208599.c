/*
 * A208599  Number of 5-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208599: Number of 5‑bead necklaces labeled with numbers -n..n
   not allowing reversal, with sum zero.
   Recurrence (valid for n > 5):
     a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
   Initial values:
     a(1)=11, a(2)=77, a(3)=291, a(4)=791, a(5)=1761
   Offset: 1
*/

#include <stddef.h>
#include <stdbool.h>

static seq_t cache[10001] = {0}; /* array big enough for n up to ~10000 */
static bool initialized = false;
static idx_t computed_upto = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety guard, offset starts at 1 */

    /* initialise base values once */
    if (!initialized) {
        cache[1] = 11ULL;
        cache[2] = 77ULL;
        cache[3] = 291ULL;
        cache[4] = 791ULL;
        cache[5] = 1761ULL;
        computed_upto = 5;
        initialized = true;
    }

    /* return already computed value */
    if (n <= computed_upto) return cache[n];

    /* extend cache up to n using the recurrence */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* the operations below use only + - * / % and are valid for
           unsigned integer types and for arbitrary‑precision classes
           that provide the usual arithmetic operators */
        cache[i] = 5 * cache[i - 1]
                 - 10 * cache[i - 2]
                 + 10 * cache[i - 3]
                 - 5 * cache[i - 4]
                 + cache[i - 5];
    }

    computed_upto = n;
    return cache[n];
}
