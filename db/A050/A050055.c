/*
 * A050055  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 4.
 * https://oeis.org/A050055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) where m = 2*n - 2 - 2^(p+1) and p is the
 *            unique integer such that 2^p < n - 1 <= 2^(p+1). Initial values:
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 125, u64 n <= 503,
 *            u128 n <= 3080, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Safety for out‑of‑range calls (n >= 1 in the OEIS definition) */
    if (n == 0) return (seq_t)0;

    /* static memoisation storage */
    static seq_t *data = NULL;
    static size_t capacity = 0;
    static size_t last_computed = 0;   /* number of terms already stored */

    /* Ensure the array can hold n terms */
    if ((size_t)n > capacity) {
        size_t newcap = capacity;
        if (newcap < 4) newcap = 4;
        while (newcap < (size_t)n) newcap <<= 1;
        data = (seq_t*)realloc(data, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* Initialise the first three terms once */
    if (last_computed == 0) {
        data[0] = 1;
        data[1] = 2;
        data[2] = 4;
        last_computed = 3;
    }

    /* Return if we already have the requested term */
    if ((size_t)n <= last_computed)
        return data[(size_t)(n - 1)];

    /* Compute missing terms up to n */
    for (idx_t curr = last_computed + 1; curr <= n; ++curr) {
        /* Find p such that 2^p < curr-1 <= 2^(p+1) */
        idx_t t = curr - 1;
        int p = 0;
        idx_t tmp = t;
        while (tmp > 1) { tmp >>= 1; ++p; }        /* floor(log2(t)) */
        if (((idx_t)1 << p) == t && p > 0) --p;    /* adjust if t is an exact power of 2 */

        /* Compute 2^(p+1) */
        seq_t pow2 = 1;
        for (int k = 0; k < p + 1; ++k) pow2 <<= 1;

        /* Compute m = 2*curr - 2 - 2^(p+1) */
        seq_t two_curr_minus2 = (seq_t)curr + (seq_t)curr - 2;
        seq_t mseq = two_curr_minus2 - pow2;
        idx_t m = (idx_t)mseq;                     /* m is in [1, curr-1] */

        /* a(curr) = a(curr-1) + a(m) */
        seq_t value = data[(size_t)(curr - 2)] + data[(size_t)(m - 1)];
        data[(size_t)(curr - 1)] = value;
    }

    last_computed = (size_t)n;
    return data[(size_t)(n - 1)];
}
