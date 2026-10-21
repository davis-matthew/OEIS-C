/*
 * A232337  Number of 3 X n 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally, with no adjacent
 *          elements equal.
 * https://oeis.org/A232337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑1)+7*a(n‑2)-3*a(n‑3)+3*a(n‑4)-7*a(n‑5)+2*a(n‑6)-2*a(n‑7)+2*a(n‑8)
   for n>12, with initial values
     a(1)=1, a(2)=16, a(3)=74, a(4)=154, a(5)=628, a(6)=1470,
     a(7)=5530, a(8)=13906, a(9)=49150, a(10)=130108,
     a(11)=439834, a(12)=1208138.
*/

#include <stddef.h>

static const seq_t init_vals[13] = {
    (seq_t)0,           /* unused, keep alignment  */
    (seq_t)1,
    (seq_t)16,
    (seq_t)74,
    (seq_t)154,
    (seq_t)628,
    (seq_t)1470,
    (seq_t)5530,
    (seq_t)13906,
    (seq_t)49150,
    (seq_t)130108,
    (seq_t)439834,
    (seq_t)1208138
};

/* cache large enough for all n that may be queried (up to ~10 000). */
#define CACHE_SIZE 20001          /* safe upper bound */
static seq_t cache[CACHE_SIZE];
static idx_t computed_max = 0;    /* highest n whose value is known */

/* initialise the cache with the first 12 terms */
static void init_cache(void) {
    if (computed_max == 0) {
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init_vals[i];
        computed_max = 12;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1, a(0) is undefined */
        return (seq_t)0;

    init_cache();

    if (n <= computed_max)
        return cache[n];

    /* compute successive terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += cache[i - 2] * (seq_t)7;
        val -= cache[i - 3] * (seq_t)3;
        val += cache[i - 4] * (seq_t)3;
        val -= cache[i - 5] * (seq_t)7;
        val += cache[i - 6] * (seq_t)2;
        val -= cache[i - 7] * (seq_t)2;
        val += cache[i - 8] * (seq_t)2;
        cache[i] = val;
    }
    computed_max = n;
    return cache[n];
}
