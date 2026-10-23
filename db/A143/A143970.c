/*
 * A143970  Eigentriangle by rows, n terms of (10 * A002535) followed by
 *          A002535(n).
 * https://oeis.org/A143970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented below:
 *
 * Let b(n) be the OEIS sequence A002535 defined by
 *    b(0) = 1
 *    b(n+1) = b(n) + 10 * (b(0) + b(1) + ... + b(n-1))   (n ≥ 0),
 * i.e. b(n+1) = b(n) + 10 * S(n), where S(n) = Σ_{i=0}^{n-1} b(i).
 *
 * For the eigen‑triangle A143970, rows are constructed as:
 *   row r  (r ≥ 0) = [10*b(0), 10*b(1), … , 10*b(r-1), b(r)]   (length r+1)
 *
 * The sequence a(n) is the concatenation of all rows.  For an index n,
 * let r be the unique integer with
 *      r(r+1)/2 ≤ n < (r+1)(r+2)/2                       (1)
 * and let k = n − r(r+1)/2.  Then
 *      a(n) = 10*b(k)          if k < r,
 *      a(n) = b(r)             if k = r.
 *
 * The implementation below follows this definition exactly, using only
 * integer arithmetic and no external libraries, and is suitable for
 * arbitrary‑precision (or 64‑bit) builds of the type seq_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Integer square‑root: floor(sqrt(n)) for 0 ≤ n < 2^64. */
static idx_t
isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    /* Use long double for initial approximation */
    long double approx = sqrt((long double)n);
    idx_t r = (idx_t)approx;          /* truncate down */
    /* Adjust upwards if we underestimated */
    while ((((__int128)r + 1) * ((__int128)r + 1)) <= n)
        ++r;
    /* Adjust downwards if we overestimated */
    while (((__int128)r * (__int128)r) > n)
        --r;
    return r;
}

/* Main sequence function a(n) */
seq_t
a(idx_t n)
{
    /* --- Find row index r from formula (1) --- */
    idx_t two_n = n * 2;
    idx_t alt = 8 * n + 1;                 /* 8*n + 1 fits in idx_t for n ≤ 10^9 */
    idx_t s = isqrt_u64(alt);
    idx_t r = (s - 1) / 2;                  /* floor((sqrt(8*n+1)-1)/2) */

    /* --- Compute offset within the row --- */
    idx_t row_start = r * (r + 1) / 2;      /* start index of row r */
    idx_t k = n - row_start;                /* 0 ≤ k ≤ r */

    /* --- Generate A002535 up to index r (and capture needed k) --- */
    seq_t b_val   = (seq_t)1;   /* b(0) */
    seq_t sum     = (seq_t)0;   /* sum of b(0..i-1) */
    seq_t b_k_val = (k == 0) ? (seq_t)1 : 0;
    seq_t b_r_val = (r == 0) ? (seq_t)1 : 0;

    for (idx_t i = 0; i < r; ++i) {
        /* b(i+1) = b(i) + 10 * sum_{j=0}^{i-1} b(j) */
        seq_t next_b = b_val + sum * (seq_t)10;
        sum += b_val;

        idx_t idx_next = i + 1;
        if (idx_next == k)
            b_k_val = next_b;
        if (idx_next == r)
            b_r_val = next_b;

        b_val = next_b;
    }

    /* --- Return the required triangle element --- */
    return (k < r) ? (b_k_val * (seq_t)10) : b_r_val;
}
