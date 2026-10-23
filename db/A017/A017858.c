/*
 * A017858  Expansion of 1/(1-x^7-x^8-x^9).
 * https://oeis.org/A017858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 94, u32 n <= 175, u64 n <= 336,
 *            u128 n <= 657, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑7) + a(n‑8) + a(n‑9) for n > 9
 * Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *                 a(6)=0, a(7)=1, a(8)=1, a(9)=1
 * (OEIS offset = 0) */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static size_t cap   = 0;       /* allocated capacity */
static idx_t  size  = 0;       /* number of computed elements */

static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cap) return;

    size_t new_cap = cap ? (cap << 1) : 1;
    while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < size; ++i) tmp[i] = cache[i];
    delete[] cache;
#else
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (n + 1 <= size) return cache[n];

    static const seq_t init[10] = {1,0,0,0,0,0,0,1,1,1};

    for (idx_t i = size; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-7] + cache[i-8] + cache[i-9];
        }
    }

    size = n + 1;
    return cache[n];
}
