/*
 * A089606  Table T(n,k), n>=1 and k>=1; the k-th row is defined by :partial
 *          sums of the sequence 1, a(1), .., a(1), a(2), .., a(2), a(3),
 *          ..,a(3), a(4), ... each term repeated k times (with a(i)= T(i,k)).
 * https://oeis.org/A089606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A089606 – diagonal enumeration of the table T(n,k)
   The sequence is built from the rows T(*,k) defined by
   T(1,k)=1,
   T(m+1,k)=T(m,k)+T(⌊(m‑1)/k⌋+1,k)   (m≥1).
   For each m≥0 let P(m)=1+m(m+1)/2.
   The block between P(m) and P(m+1) (length m) contains the terms
       T(m−i+1 , i+1)   for i=1…m,
   and the term at P(m) itself is 2^m.
   The function below implements exactly this description.  */
#include <stdint.h>

#define MAX_ROW 2048   /* safe for all requested n (up to ~10^4) */

/* triangular numbers: P(m)=1+m(m+1)/2 */
static idx_t triangular(idx_t m) {
    return 1 + (idx_t)(((unsigned __int128)m * (m + 1)) / 2);
}

/* 2^e computed by repeated doubling (works for any seq_t type) */
static seq_t pow2_seq(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) {
        r += r;
    }
    return r;
}

/* value T(a,k) using the row recurrence */
static seq_t T_value(idx_t a, idx_t k) {
    seq_t row[MAX_ROW + 1];
    row[1] = 1;
    for (idx_t r = 2; r <= a; ++r) {
        idx_t idx = ((r - 2) / k) + 1;
        row[r] = row[r - 1] + row[idx];
    }
    return row[a];
}

seq_t a(idx_t n) {
    /* find the largest m with P(m) <= n */
    idx_t m = 0;
    while (triangular(m) < n) ++m;
    if (triangular(m) > n) --m;

    idx_t block_start = triangular(m);   /* position of 2^m */

    if (n == block_start) {
        return pow2_seq(m);
    }

    /* inside the block */
    idx_t i = n - block_start;   /* 1 … m */
    idx_t a_val = m - i + 1;     /* row index in T */
    idx_t k_val = i + 1;         /* column index in T */

    return T_value(a_val, k_val);
}
