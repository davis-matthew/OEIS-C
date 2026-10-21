/*
 * A081954  Triangle read by rows: T(n, k) = 2^(n-k)*3^k, n >= 1, 0 <= k < n.
 * https://oeis.org/A081954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 64, u32 n <= 229, u64 n <= 860,
 *            u128 n <= 3400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^(r-k) · 3^k
 *  where r is the unique row index such that r(r+1)/2 ≥ n
 *  and k = n - r(r-1)/2 - 1 is the column index (k = 0 … r-1).
 *  The sequence is obtained by reading T(r,k) row‑by‑row.
 *
 *  Algorithm: 1. Determine r by solving r(r+1)/2 ≥ n
 *              (using an integer square root and a small correction).
 *            2. Compute column k from the offset within the row.
 *            3. Evaluate 2^(r-k) and 3^k by repeated squaring on seq_t.
 *            4. Return the product.
 */
#include <stdint.h>

typedef uint64_t idx_t; /* the build supplies the actual type */

/* ------------------------------------------------------------------ */
/* integer square root (floor) for 64 bit unsigned integers            */
/* ------------------------------------------------------------------ */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* fast exponentiation for seq_t (power by squaring)                  */
/* ------------------------------------------------------------------ */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --------- compute row r ------------------------------------- */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    if ((__int128)r * (r + 1) / 2 < n)
        ++r;

    /* --------- compute column k ----------------------------------- */
    idx_t prev_row_total = r * (r - 1) / 2;          /* elements before row r */
    idx_t k = n - prev_row_total - 1;                /* 0‑based column */

    /* --------- evaluate 2^(r-k) and 3^k -------------------------- */
    idx_t exp2 = r - k;
    idx_t exp3 = k;
    seq_t pow2 = pow_seq_t((seq_t)2, exp2);
    seq_t pow3 = pow_seq_t((seq_t)3, exp3);

    /* --------- final value --------------------------------------- */
    return pow2 * pow3;
}
