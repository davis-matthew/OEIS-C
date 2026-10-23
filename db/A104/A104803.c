/*
 * A104803  a(n) = floor(sqrt(a(n-1)^2 + a(n-2)^2)), a(1)=1, a(2)=3.
 * https://oeis.org/A104803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( a(n-1)^2 + a(n-2)^2 ) ), a(1)=1, a(2)=3
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for arbitrary‑precision or built‑in unsigned type. */
static seq_t isqrt_seq(seq_t x)
{
    if (x < 2)
        return x;

    seq_t result = 0;
    seq_t bit = 1;

    /* Find the largest power of four that is <= x. */
    while (bit <= x)
        bit <<= 2;
    bit >>= 2;

    while (bit != 0) {
        if (x >= result + bit) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

/* 128‑bit integer square root (used only when seq_t is 64‑bit or smaller). */
static seq_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 mid, sq, res = 0;

    while (low <= high) {
        mid = (low + high) >> 1;
        sq  = mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)res;
}

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; we ignore index 0. */
    if (n < 1)
        return 0;           /* undefined region */

    static seq_t *seq = NULL;
    static idx_t size   = 0;   /* highest index that has been computed */
    static idx_t capacity = 0;

    /* Resize the array if needed. We need space up to n (1‑based). */
    if (n + 1 > capacity) {
        idx_t newcap = (capacity == 0) ? 4 : capacity << 1;
        while (newcap <= (idx_t)n)          /* ensure newcap > n */
            newcap <<= 1;

        seq_t *newptr;
#if defined(__cplusplus)
        newptr = new seq_t[newcap];
#else
        newptr = (seq_t *)malloc(sizeof(seq_t) * newcap);
#endif
        if (!newptr)
            return 0;                    /* allocation failure */

        /* Copy old values. */
        for (idx_t i = 1; i <= size; ++i)
            newptr[i] = seq[i];

#if defined(__cplusplus)
        delete[] seq;
#else
        free(seq);
#endif
        seq = newptr;
        capacity = newcap;
    }

    /* Initialise the first two terms if they are not yet present. */
    if (size < 1) {
        seq[1] = (seq_t)1;
        size   = 1;
    }
    if (size < 2) {
        seq[2] = (seq_t)3;
        size   = 2;
    }

    /* Compute all terms up to n. */
    for (idx_t i = size + 1; i <= n; ++i) {
        if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller – use 128‑bit arithmetic */
            unsigned __int128 a1 = (unsigned __int128)seq[i - 1];
            unsigned __int128 a2 = (unsigned __int128)seq[i - 2];
            unsigned __int128 sumsq = a1 * a1 + a2 * a2;
            seq[i] = isqrt_uint128(sumsq);
        } else {                 /* arbitrary‑precision type */
            seq_t a1 = seq[i - 1];
            seq_t a2 = seq[i - 2];
            seq_t sumsq = a1 * a1 + a2 * a2;
            seq[i] = isqrt_seq(sumsq);
        }
    }
    size = n;
    return seq[n];
}
