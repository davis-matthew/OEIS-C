/*
 * A208218  a(n)=(a(n-1)^2*a(n-3)+a(n-2))/a(n-4) with a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A208218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^2 * a(n-3) + a(n-2)) / a(n-4) * with initial
 *            values a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#ifndef A208218_MAX_N
#define A208218_MAX_N 10000   /* sufficient for n up to ~10000 */
#endif

/* 4 initial terms are known; the rest start at 0 so they will be computed. */
static seq_t cache[A208218_MAX_N + 1] = {1, 1, 1, 1};

/* safe multiplication: use intrinsic 128‑bit arithmetic on C builds,
   otherwise rely on the type’s operator*. */
#ifdef __cplusplus
static inline seq_t mp_mul(seq_t a, seq_t b) { return a * b; }
#else
static inline seq_t mp_mul(seq_t a, seq_t b)
{
    return (seq_t)((unsigned __int128)a * b);
}
#endif

seq_t a(idx_t n)
{
    /* Immediate return for the trivial first four terms. */
    if (n <= 3) return 1;

    /* If n is within the preallocated cache, use it. */
    if (n <= A208218_MAX_N) {
        for (idx_t i = 4; i <= n; ++i) {
            if (cache[i]) continue;                /* already computed */
            seq_t sq   = mp_mul(cache[i-1], cache[i-1]); /* a(i-1)^2         */
            seq_t prod = mp_mul(sq, cache[i-3]);        /* * a(i-3)         */
            seq_t num  = prod + cache[i-2];             /* + a(i-2)         */
            cache[i]   = num / cache[i-4];              /* divide by a(i-4)  */
        }
        return cache[n];
    }

    /* For n beyond the static cache, compute iteratively without caching. */
    seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 1, an = 1;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t sq   = mp_mul(a1, a1);     /* a(i-1)^2        */
        seq_t prod = mp_mul(sq, a0);     /* * a(i-3)        */
        seq_t num  = prod + a2;          /* + a(i-2)        */
        an = num / a3;                   /* / a(i-4)        */

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
