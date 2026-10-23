/*
 * A180572  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in the circular ladder P_2 X C_n (also
 *          called a prism), where P_2 is the path graph on 2 nodes and C_n is
 *          the cycle graph on n nodes.
 * https://oeis.org/A180572
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑3)-th element (0‑based) of the triangle T(r,k)
 *            described in OEIS A180572. For a row r≥3, – T(r,1)=3r (edges), –
 *            for 2≤k≤1+⌊r/2⌋ let m=⌊r/2⌋: same‑row pairs = (k≤m)? ((r even &&
 *            k==m)? r : 2*r) : 0 cross‑row pairs=(k-1≤m)?((r even && k-1==m)?
 *            r : 2*r) : 0 – a(n)=same‑row + cross‑row
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum of floor(i/2) for i = 1..r */
static inline seq_t sum_floor(idx_t r)
{
    idx_t m = r >> 1;                 /* m = floor(r/2)           */
    if ((r & 1) == 0)                 /* r even                    */
        return (seq_t)m * (seq_t)m;
    else                              /* r odd                     */
        return (seq_t)m * (seq_t)m + (seq_t)m;
}

/* Index of the last element of row r (0‑based offset from a(3)) */
static inline seq_t S_of(idx_t r)
{
    seq_t f = sum_floor(r);          /* F(r)                       */
    return (seq_t)(r - 3) + f;       /* S(r) = (r-3) + F(r)        */
}

/* Triangle entry T(r,k) for r≥3, 1≤k≤1+⌊r/2⌋ */
static inline seq_t T_row_k(idx_t r, idx_t k)
{
    if (k == 1)                       /* edges                     */
        return (seq_t)3 * (seq_t)r;

    idx_t half = r >> 1;              /* floor(r/2)                */

    seq_t same = 0, cross = 0;
    if (k <= half) {
        if ((r & 1) == 0 && k == half) /* n even and d == n/2       */
            same = (seq_t)r;
        else
            same = (seq_t)2 * (seq_t)r;
    }

    idx_t d = k - 1;                   /* d0 = k-1                  */
    if (d <= half) {
        if ((r & 1) == 0 && d == half)
            cross = (seq_t)r;
        else
            cross = (seq_t)2 * (seq_t)r;
    }

    return same + cross;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                    /* below offset – undefined */

    idx_t idx = n - 3;                /* 0‑based position in the triangle */

    idx_t r = 3;                      /* current row   */
    for (;; ++r) {                    /* find row containing idx   */
        seq_t s = S_of(r);
        if (s > idx)
            break;
    }

    seq_t prev = (r == 3) ? 0 : S_of(r - 1);
    idx_t offset = idx - (idx_t)prev;  /* 0‑based index inside row  */
    idx_t k = offset + 1;              /* column number             */

    return T_row_k(r, k);
}
