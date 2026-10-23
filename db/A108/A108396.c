/*
 * A108396  Triangle read by rows: T(n,k) = n*(1+n^k)/2, 0<=k<=n.
 * https://oeis.org/A108396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the (n‑th) element of the triangle read by rows * T(n,k)
 *            = n * (1 + n^k) / 2 , 0 ≤ k ≤ n * This implementation first
 *            determines the row r and column k for * index n, then computes
 *            n^k using a 128‑bit intermediate when seq_t * is 64‑bit to avoid
 *            overflow, and finally evaluates the result *...
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 63, u64 n <= 151,
 *            u128 n <= 404, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for n ≥ 0 */
seq_t a(idx_t n)
{
    /* --- locate row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 --- */
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t c = n - r * (r + 1) / 2;          /* column index (k) */

    /* --- compute r^c --- */
    seq_t pow;
    if (sizeof(seq_t) <= 8) {               /* seq_t is 64‑bit or less */
        unsigned __int128 p = 1;
        for (idx_t i = 0; i < c; ++i)
            p *= (unsigned __int128)r;
        pow = (seq_t)p;                     /* truncate to width of seq_t */
    } else {                                /* arbitrary‑precision seq_t */
        pow = 1;
        for (idx_t i = 0; i < c; ++i)
            pow *= (seq_t)r;
    }

    /* --- evaluate T(r,c) = r * (1 + r^c) / 2 without intermediate overflow --- */
    seq_t result;
    if ((r & 1) == 0) {                     /* r even */
        seq_t a_part = r >> 1;              /* r / 2 */
        seq_t b_part = pow + 1;             /* 1 + r^c */
        result = a_part * b_part;
    } else {                                /* r odd, 1 + r^c is even */
        seq_t a_part = r;                   /* r */
        seq_t b_part = (pow + 1) >> 1;      /* (1 + r^c) / 2 */
        result = a_part * b_part;
    }
    return result;
}
