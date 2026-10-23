/*
 * A164644  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n ≥ 8
     a(n) = 25·a(n‑1) + 25·a(n‑2) + 25·a(n‑3) + 25·a(n‑4) + 25·a(n‑5) + 25·a(n‑6)
            – 325·a(n‑7)
   For n = 7 the constant term from the numerator of the generating function
   contributes a +1: a(7) = 25·(a6+…+a1) – 325·a0 + 1.
   Initial values:
     a(0)=1, a(1)=27, a(2)=702, a(3)=18252, a(4)=474552,
     a(5)=12338352, a(6)=320797152
*/

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;      /* array of a(n) values */
static size_t cache_len = 0;     /* number of values currently stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Initial terms for n = 0 … 6 */
static const seq_t init[7] = {
    (seq_t)1,
    (seq_t)27,
    (seq_t)702,
    (seq_t)18252,
    (seq_t)474552,
    (seq_t)12338352,
    (seq_t)320797152
};

/* Grow the cache to accommodate values up to 'n' inclusive */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;              /* already enough */

    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)          /* allocation failure is fatal */
        abort();

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to n */
    ensure_capacity(n);

    /* Fill missing values lazily */
    while (cache_len <= (size_t)n) {
        if (cache_len < 7) {
            /* Initial terms */
            cache[cache_len] = init[cache_len];
        } else if (cache_len == 7) {
            /* a(7): recurrence + constant term from numerator */
            seq_t sum = 0;
            for (int i = 1; i <= 6; ++i)
                sum += (seq_t)25 * cache[cache_len - i];
            sum -= (seq_t)325 * cache[cache_len - 7];
            sum += (seq_t)1;                      /* the +1 term */
            cache[cache_len] = sum;
        } else {
            /* Recurrence for n ≥ 8 */
            seq_t sum = 0;
            for (int i = 1; i <= 6; ++i)
                sum += (seq_t)25 * cache[cache_len - i];
            sum -= (seq_t)325 * cache[cache_len - 7];
            cache[cache_len] = sum;
        }
        ++cache_len;
    }

    return cache[n];
}
