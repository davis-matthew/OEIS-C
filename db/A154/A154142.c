/*
 * A154142  Indices k such that 9 plus the k-th triangular number is a perfect
 *          square.
 * https://oeis.org/A154142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where 9 + k*(k+1)/2 is a perfect square. Recurrence:
 *            a(1)=0, a(2)=10, a(3)=13, a(4)=63, a(n) = 6*a(n-2) - a(n-4) + 2
 *            (n>4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;        /* array storing computed values   */
static size_t a_cache_size = 0;      /* number of values actually stored */
static size_t a_cache_cap  = 0;      /* allocated capacity              */

/* Ensure that a_cache can hold at least "need" elements.         */
static void ensure_capacity(size_t need)
{
    if (need <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure: leave cache unchanged */
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Compute and cache all values up to index n (1‑based).               */
static void compute_up_to(idx_t n)
{
    if (n <= a_cache_size) return;              /* already available */

    size_t need = (size_t)n;
    ensure_capacity(need);

    for (size_t i = a_cache_size; i < need; ++i)   /* i is 0‑based index of a(i+1) */
    {
        seq_t val;
        if (i < 4)                              /* first four terms are fixed */
        {
            switch (i)
            {
                case 0: val = (seq_t)0; break;
                case 1: val = (seq_t)10; break;
                case 2: val = (seq_t)13; break;
                case 3: val = (seq_t)63; break;
            }
        }
        else                                     /* recurrence for i>=4 */
        {
            val = (seq_t)6 * a_cache[i-2] - a_cache[i-4] + (seq_t)2;
        }
        a_cache[i] = val;
    }
    a_cache_size = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, but guard against 0 */
    compute_up_to(n);
    return a_cache[(size_t)(n-1)];     /* array index is n-1 */
}
