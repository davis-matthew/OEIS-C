/*
 * A186244  Number of ternary strings of length n containing 00.
 * https://oeis.org/A186244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186244: Number of ternary strings of length n containing 00
   Recurrence: a(0)=0, a(1)=0, a(2)=1, a(n)=2*a(n-1)+2*a(n-2)+3^(n-2) for n>=3 */
#include <stdlib.h>
#include <stdint.h>

/* Cached values of a(n) */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;   /* allocated array size */
static size_t a_len   = 0;   /* next free index */

/* Cached power of 3: pow3 = 3^pow3_exp */
static seq_t pow3      = (seq_t)1;
static idx_t pow3_exp  = 0;  /* current exponent satisfying pow3 = 3^pow3_exp */

/* Grow a_cache to hold indices up to n (inclusive) */
static void grow_a(idx_t n)
{
    if ((size_t)n + 1 <= a_cap) return;                /* already enough */

    /* Allocate the minimum needed space (round up a bit for efficiency) */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_a) exit(EXIT_FAILURE);
    a_cache = new_a;
    a_cap   = new_cap;
}

/* Initialise caches on first call */
static void init_caches(void)
{
    if (a_len != 0) return;                  /* already initialised */

    /* Base values */
    grow_a(2);
    a_cache[0] = (seq_t)0;
    a_cache[1] = (seq_t)0;
    a_cache[2] = (seq_t)1;
    a_len = 3;

    /* Initial power: pow3 = 3^0 = 1, pow3_exp = 0 */
    pow3 = (seq_t)1;
    pow3_exp = 0;
}

/* Compute a(n).  O(n) cumulative time over all calls. */
seq_t a(idx_t n)
{
    init_caches();

    if (n == 0) return a_cache[0];
    if (n == 1) return a_cache[1];
    if (n == 2) return a_cache[2];

    /* Ensure we have values up to n */
    if ((size_t)n >= a_len) {
        /* Maintain pow3 to 3^(i-2) for each i as we fill */
        for (idx_t i = a_len; i <= n; ++i) {
            /* Increase pow3 until it equals 3^(i-2) */
            while (pow3_exp < i - 2) {
                pow3     *= (seq_t)3;
                pow3_exp += 1;
            }

            /* a(i) = 2*a(i-1) + 2*a(i-2) + 3^(i-2) */
            seq_t val = a_cache[i - 1] * (seq_t)2;
            val      += a_cache[i - 2] * (seq_t)2;
            val      += pow3;

            /* Store computed value */
            grow_a(i);
            a_cache[i] = val;
            a_len++;
        }
    }

    return a_cache[(size_t)n];
}
