/*
 * A106026  A fractal transform of Pell numbers : a(1)=1 then if
 *          b(n)<k<=b(n+1) a(k)=b(n+1)-a(k-b(n)) where b(n)=A000129(n).
 * https://oeis.org/A106026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For k > 1, let n be such that b(n) < k ≤ b(n+1) where
 *            b(m) = A000129(m) (the m‑th Pell number, with b(1)=1, b(2)=2,
 *            …). Then a(k) = b(n+1) – a(k – b(n)). This implements exactly
 *            that definition
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* safety, offset is 1 */
    /* Allocate DP array for a[1..n] */
    seq_t *dp = (seq_t*)malloc(sizeof(seq_t)*(n+1));
    if (!dp) return 0;                   /* allocation failed */
    dp[1] = 1;

    /* Pre‑compute Pell numbers b(1)…b(m) with b(m) ≥ n.
       b(1)=1, b(2)=2, and b(k) = 2*b(k-1) + b(k-2).    */
    seq_t B[64];                 /* enough for n up to at least 1e9 */
    unsigned B_len = 2;          /* current number of elements stored */
    B[1] = 1;
    B[2] = 2;
    while (B[B_len] < n) {
        seq_t next = 2*B[B_len] + B[B_len-1];
        ++B_len;
        B[B_len] = next;
    }

    unsigned seg = 1;          /* segment index n such that b(n) < k ≤ b(n+1) */
    for (idx_t k = 2; k <= n; ++k) {
        while (k > B[seg+1]) ++seg;          /* move to the correct segment */
        idx_t diff = k - (idx_t)B[seg];      /* k - b(n) */
        dp[k] = B[seg+1] - dp[diff];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
