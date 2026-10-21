/*
 * A095981  Number of plateau-free Motzkin paths of length n.
 * https://oeis.org/A095981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-2) + 1 +
 *              Σ_{k=2}^{n-2} a(k)*(1 + a(n-2-k)),
 *  with initial values a(0)=0, a(1)=0, a(2)=1, a(3)=2,
 *  a(4)=5, a(5)=11, a(6)=26. (Recurrence for n≥3)
 */

#include <stdint.h>

#ifndef MAX_N
#define MAX_N 20000
#endif

/* External linkage required */
seq_t a(idx_t n)
{
    static seq_t cache[MAX_N];
    static idx_t computed = 0;
    static int initialized = 0;

    if (!initialized) {
        /* set known initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)11;
        cache[6] = (seq_t)26;
        computed = 6;
        initialized = 1;
    }

    if (n <= computed)
        return cache[n];

    /* guard against request beyond pre‑allocated space */
    if (n >= MAX_N) {
        /* Sequence grows too large for this static array size */
        /* The implementation cannot handle such a request */
        return (seq_t)0; /* return value ignored by the caller */
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 2] + (seq_t)1;

        /* accumulate Σ_{k=2}^{i-2} a(k)*(1 + a(i-2-k)) */
        for (idx_t k = 2; k <= i - 2; ++k) {
            val += cache[k] * (cache[i - 2 - k] + (seq_t)1);
        }

        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
