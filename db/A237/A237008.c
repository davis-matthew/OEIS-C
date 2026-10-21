/*
 * A237008  Number of (n+1)X(1+1) 0..3 arrays with the sum of all four
 *          elements of every 2X2 subblock equal.
 * https://oeis.org/A237008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237008: Number of (n+1)×2 arrays over {0,1,2,3} with equal
   2×2 subblock sums.  For columns i=0..n, let s_i = a_i+b_i.
   The condition forces s_i+s_{i+1}=C constant, hence
   s_even = s₀, s_odd = C−s₀, so columns of even/odd indices
   share the same pair-sum.  Let f(s) be the number of
   (x,y)∈[0,3]² with x+y=s:
      f = {1,2,3,4,3,2,1} for s=0..6.
   For n columns of parity counts e=⌊(n+2)/2⌋, o=⌊(n+1)/2⌋,
   the number of assignments is
       A(e)·A(o)
   where A(k)=∑_{s=0}^6 f(s)^k.  The values A(k) are precomputed
   up to the required maximum. */
#include <stdlib.h>

/* Precomputed per-sum counts */
static const seq_t f[7] = {1, 2, 3, 4, 3, 2, 1};

static seq_t *A = NULL;          /* A[k] = Σ_s f[s]^k  */
static size_t A_cnt = 0;         /* number of computed entries (A[0]..A[A_cnt-1]) */
static seq_t cur_pow[7];         /* current powers f[s]^k during extension */

seq_t a(idx_t n)
{
    /* parity column counts */
    idx_t e = (n + 2) / 2;       /* even index columns count   */
    idx_t o = (n + 1) / 2;       /* odd  index columns count   */
    idx_t kmax = e > o ? e : o;  /* maximum k we need */

    /* initialise cache on first call */
    if (!A) {
        A_cnt = 1;            /* we will compute A[0] */
        A = (seq_t *)malloc(sizeof(seq_t) * A_cnt);
        if (!A)
            return (seq_t)0; /* allocation failure (should not happen) */

        /* A[0] = sum f[s]^0 = 7 */
        A[0] = 7;
        for (int i = 0; i < 7; ++i)
            cur_pow[i] = 1;
    }

    /* extend cache if necessary */
    if ((size_t)kmax >= A_cnt) {
        size_t new_cnt = (size_t)kmax + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cnt * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure */
        A = tmp;
        for (idx_t k = A_cnt; k <= kmax; ++k) {
            /* update powers */
            for (int s = 0; s < 7; ++s)
                cur_pow[s] *= f[s];
            /* sum powers to get A[k] */
            seq_t sum = 0;
            for (int s = 0; s < 7; ++s)
                sum += cur_pow[s];
            A[k] = sum;
        }
        A_cnt = new_cnt;
    }

    /* result is product A[e] * A[o] */
    return A[e] * A[o];
}
