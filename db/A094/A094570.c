/*
 * A094570  Triangle T(n,k) read by rows: T(n,k) = F(k) + F(n-k) where F(n) is
 *          the n-th Fibonacci number.
 * https://oeis.org/A094570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = F(k) + F(n‑k), where T(n,k) = F(k)+F(n-k) is read * by
 *            rows. Here F(0)=0, F(1)=1. The sequence is obtained by *
 *            flattening the triangle row‑by‑row. * Example: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=2, …
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---- fast Fibonacci by binary doubling (F(0)=0, F(1)=1) ---- */
static inline seq_t fib_fast(idx_t n)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* find highest set bit of n */
    idx_t mask = (idx_t)1 << (sizeof(idx_t)*8 - 1);
    while (mask && !(mask & n)) mask >>= 1U;

    for (; mask; mask >>= 1U) {
        /* c = F(2k) = a * (2*b - a) */
        seq_t c = a * ((b << 1) - a);
        /* d = F(2k+1) = a*a + b*b */
        seq_t d = a * a + b * b;
        if (n & mask) {          /* odd: (F(2k+1), F(2k+2)) */
            a = d;
            b = c + d;
        } else {                 /* even: (F(2k), F(2k+1)) */
            a = c;
            b = d;
        }
    }
    return a;   /* F(n) */
}

/* ---- find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 ---- */
static inline idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* increase high until its start index exceeds n */
    while ((((__int128)high * (high + 1)) >> 1) <= n) {
        high <<= 1U;
    }

    /* binary search on [low, high) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((((__int128)mid * (mid + 1)) >> 1) <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;   /* last r with start ≤ n */
}

/* ---- sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r      = find_row(n);
    idx_t start  = (idx_t)(((__int128)r * (r + 1)) >> 1);
    idx_t j      = n - start;       /* column index in row r */

    seq_t f1 = fib_fast(j);
    seq_t f2 = fib_fast(r - j);
    return f1 + f2;
}
