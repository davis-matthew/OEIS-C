/*
 * A194883  Write n = C(i,4)+C(j,3)+C(k,2)+C(l,1) with i>j>k>l>=0; sequence
 *          gives j values.
 * https://oeis.org/A194883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j where n = C(i,4)+C(j,3)+C(k,2)+C(l,1) with i>j>k>l>=0
 *            is represented uniquely by the combinatorial number system of
 *            degree 4, and a(n) returns the j component of that
 *            representation
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute binomial coefficient C(x,4) as an unsigned integer.  Uses
   a 128‑bit intermediate to avoid overflow when seq_t is less than 128 bits. */
static inline seq_t binom4(idx_t x)
{
    if (x < 4) return 0;
    unsigned __int128 v = 1;
    for (idx_t m = 1; m <= 4; ++m)
        v = v * (x - 4 + m) / m;
    return (seq_t)v;
}

/* Compute binomial coefficient C(x,3). */
static inline seq_t binom3(idx_t x)
{
    if (x < 3) return 0;
    unsigned __int128 v = 1;
    for (idx_t m = 1; m <= 3; ++m)
        v = v * (x - 3 + m) / m;
    return (seq_t)v;
}

/* Compute binomial coefficient C(x,2). */
static inline seq_t binom2(idx_t x)
{
    if (x < 2) return 0;
    return (seq_t)(((unsigned __int128)x * (x - 1)) / 2);
}

/* Return the j component of the unique representation of n in the
   combinatorial number system of degree 4. */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;

    /* --- Find i: largest with C(i,4) <= target --- */
    idx_t i = 0;
    for (;; ++i) {
        seq_t c = binom4(i);
        if (c > target) break;
    }
    --i;
    target -= binom4(i);

    /* --- Find j: largest < i with C(j,3) <= target --- */
    idx_t j = (i > 0) ? (i - 1) : 0;
    for (;; --j) {
        seq_t c = binom3(j);
        if (c <= target) break;
    }
    target -= binom3(j);

    /* --- Find k: largest < j with C(k,2) <= target --- */
    idx_t k = (j > 0) ? (j - 1) : 0;
    for (;; --k) {
        seq_t c = binom2(k);
        if (c <= target) break;
    }
    target -= binom2(k);

    /* l = target is not needed for the return value. */
    (void)target;   /* silence unused variable warning */

    return (seq_t)j;
}
