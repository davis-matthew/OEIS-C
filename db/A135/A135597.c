/*
 * A135597  Square array read by antidiagonals: row m (m >= 1) satisfies b(0)
 *          = b(1) = 1; b(n) = m*b(n-1) + b(n-2).
 * https://oeis.org/A135597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   b(0) = b(1) = 1, b(n) = m*b(n‑1) + b(n‑2) for n ≥ 2 a(n) is the
 *            antidiagonal read of the infinite square array whose rows are
 *            the sequences b(·) for m = 1,2,3,…
 * Range:     u8 n <= 41, u16 n <= 86, u32 n <= 201, u64 n <= 484,
 *            u128 n <= 1209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------- */

#include <stdint.h>   /* for uint64_t used by utils, if needed */

/* ----------------------------------------------------------- */
/* Integer square root (floor of sqrt(x)), 0 ≤ x < 2^64.
   Uses only integer arithmetic and __int128 for safe multiplication.   */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 31;          /* start with the highest possible bit (2^31) */
    while (bit != 0) {
        idx_t next = r | bit;
        __int128 prod = (__int128)next * next;
        if (prod <= x) r = next;
        bit >>= 1;
    }
    return r;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)           /* n ≥ 1, 1‑based indexing as in OEIS */
{
    /* 1. Find the antidiagonal number d (sum m + c) such that
       d*(d+1)/2 ≥ n and (d-1)*d/2 < n.  */
    idx_t s = 1 + 8 * n;                         /* = 1 + 8n */
    idx_t sqrt_s = floor_sqrt_u64(s);            /* floor(sqrt(1+8n)) */
    idx_t d = (sqrt_s - 1) / 2;                  /* floor((sqrt‑1)/2) */
    if (d * (d + 1) / 2 < n) d++;                /* adjust to nearest ceil */

    /* 2. Compute offset inside the chosen diagonal. */
    idx_t terms_before = d * (d - 1) / 2;        /* terms before this diagonal */
    idx_t k = n - terms_before - 1;             /* 0‑based offset within the diagonal */

    /* 3. Determine row (m) and column (c) for this entry. */
    idx_t m = d - k;         /* row index, 1‑based */
    idx_t c = k;             /* column index, 0‑based */

    /* 4. Evaluate the b(c) value for row m. */
    if (c == 0 || c == 1) return (seq_t)1;     /* b(0)=b(1)=1 */

    seq_t prev2 = 1;        /* b(0) */
    seq_t prev1 = 1;        /* b(1) */
    seq_t cur    = 1;       /* placeholder */

    for (idx_t i = 2; i <= c; ++i) {
        cur = (seq_t)m * prev1 + prev2;   /* b(i) = m*b(i‑1) + b(i‑2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
