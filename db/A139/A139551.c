/*
 * A139551  Triangle read by rows: T(n,k) = if n>=3*k and n<3*k*A014963(k-1)
 *          then k else 1 T(n,0)=1.
 * https://oeis.org/A139551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the (n‑th entry, read row by row) of the triangle
 * defined by 
 *   T(n,0) = 1
 *   T(n,k) = k  if n >= 3*k  and  n < 3*k*A014963(k-1)
 *          = 1  otherwise,
 * where A014963(m) = 2^m (the powers of two). 
 * Thus an entry T(r,k) is k iff r>=3k and r<3k·2^(k-1), else 1.
 *
 * The sequence a(n) itself is the list of those triangle entries
 * flattened row by row (row 0 has 1 entry, row 1 has 2 entries, etc.).
 *
 * -------------------------------------------------------------
 * The function below implements the above definition using only
 * integer arithmetic, works for any seq_t size specified by the
 * build, and is O(sqrt(n)) per call (which is < 150 iterations for
 * n <= 10,000).
 */

#ifndef _A139551_H_
#define _A139551_H_

/*  Helper: compute the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static inline idx_t row_from_index(idx_t n) {
    idx_t r = 0;
    /* find the largest r with r*(r+1)/2 <= n */
    while ((idx_t)(r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    return r;
}

/*  Main function required by the framework */
seq_t a(idx_t n) {
    /* Find the row r and the column k of the n-th flattened entry */
    idx_t r = row_from_index(n);
    idx_t start_idx = r * (r + 1) / 2;      /* sum of lengths of rows < r */
    idx_t k = n - start_idx;                /* column index in this row (0‑based) */

    /* T(r,0) is always 1 */
    if (k == 0) {
        return (seq_t)1;
    }

    /* Check the condition for a non‑trivial entry */
    /* Need r >= 3*k first */
    if ((idx_t)r >= 3 * k) {
        /* Determine whether r < 3*k*2^(k-1) */
        /* Avoid overflow by using the equivalent test
           2^(k-1) > floor(r/(3*k))  */
        idx_t divisor = r / (3 * k);   /* floor division; divisor <= 200/3 < 100 */

        /* For very large k the power 2^(k-1) is astronomically large,
           and obviously greater than divisor; we can shortcut. */
        if (k >= 64) {                 /* k-1 >= 63; shift would be UB */
            return (seq_t)k;
        }

        /* Compute 2^(k-1) safely in 64‑bit unsigned arithmetic */
        unsigned long long pow_two = 1ULL << (k - 1);

        /* If 2^(k-1) > divisor → the upper bound is satisfied */
        if (pow_two > divisor) {
            return (seq_t)k;
        }
    }

    /* Otherwise the entry is 1 */
    return (seq_t)1;
}

#endif /* _A139551_H_ */
