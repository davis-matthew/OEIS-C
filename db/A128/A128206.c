/*
 * A128206  Inverse of number triangle A128207.
 * https://oeis.org/A128206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n = r*(r+1)/2 + r * a(n) = fib(r) if n = r*(r+1)/2 +
 *            (r-1) * a(n) = 0 otherwise * where r is the unique integer with
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. The * sequence A128206 is the
 *            anti‑diagonal (in‐version) of the number * triangle A128207
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    /* Find r such that T(r) = r*(r+1)/2 ≤ n < T(r+1).  Binary search. */
    idx_t low = 0;
    idx_t high = 1;
    /* Expand high until T(high) > n. */
    while ( ((unsigned __int128)high * (high + 1)) / 2 <= n )
        high <<= 1;
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t fib_fast(idx_t k)
{
    seq_t a = 0;  /* F(0) */
    seq_t b = 1;  /* F(1) */
    /* Iterate over the bits of k from most significant to least. */
    for (int i = 63 - __builtin_clzll(k); i >= 0; --i) {
        /* Compute F(2m) = a*(2*b−a) and F(2m+1) = a*a + b*b */
        seq_t c = a * (((seq_t)2 * b) - a);
        seq_t d = a * a + b * b;
        if ((k >> i) & 1) {
            a = d;
            b = c + d;
        } else {
            a = c;
            b = d;
        }
    }
    return a; /* F(k) */
}

seq_t a(idx_t n)
{
    idx_t r   = find_row(n);
    uint64_t start128 = (uint64_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t    start   = (idx_t)start128;
    idx_t    j       = n - start;          /* column within row r */

    if (j == r)                           /* main diagonal */
        return (seq_t)1;
    if (r > 0 && j == r - 1)              /* sub‑diagonal */
        return fib_fast(r);
    return (seq_t)0;                      /* all other positions are zero */
}
