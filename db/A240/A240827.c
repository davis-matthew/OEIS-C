/*
 * A240827  a(n) = n for 1<=n<=6; thereafter a(n) = a(n-a(n-3))+a(n-a(n-6)).
 * https://oeis.org/A240827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(1)=1,a(2)=2,a(3)=3,a(4)=4,a(5)=5,a(6)=6;
 *            a(n)=a(n-a(n-3))+a(n-a(n-6)) for n>=7 (OEIS A240827)
 * Range:     u8 n <= 486, u16 n <= 9538, u32 n <= 9538, u64 n <= 9538,
 *            u128 n <= 9538, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* 1‑based, cache[0] will be 0 */
static idx_t cap = 0;         /* current capacity (>=1) */
static idx_t last = 0;        /* largest index with a computed value */

/* ensure cache can hold up to n (inclusive) */
static void ensure_cap(idx_t n)
{
    if (cap > n) return;
    idx_t new_cap = cap ? cap << 1 : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */
    cache = tmp;
    if (cap == 0) {                   /* initial allocation */
        cache[0] = 0;
    }
    cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not an indexed term, but safe */

    ensure_cap(n);
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = (seq_t)i;        /* base values */
            } else {
                /* indices for the recurrence */
                idx_t idx1 = (i > (idx_t)cache[i-3]) ? i - (idx_t)cache[i-3] : 0;
                idx_t idx2 = (i > (idx_t)cache[i-6]) ? i - (idx_t)cache[i-6] : 0;
                cache[i] = cache[idx1] + cache[idx2];
            }
        }
        last = n;
    }
    return cache[n];
}
