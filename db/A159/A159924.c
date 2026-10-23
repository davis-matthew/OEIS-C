/*
 * A159924  Triangle read by rows: a(m,m) = 1, for all m. For n < m, a(m,n) =
 *          a(m-1,n) + (sum of all terms in rows 1 through m-1).
 * https://oeis.org/A159924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(m,m) = 1, a(m,n) = a(m-1,n) + S_{m-1} (for n < m), where S_t =
 *            sum of all terms in rows 1..t. With the derived identities:
 *            RowSum_t = t * S_{t-1} + 1, S_t = S_{t-1} + RowSum_t, RowSum_1 =
 *            1, S_1 = 1. Then for n in row m, column k (1 ≤ k ≤ m): if k ==
 *            m: a = 1 else a = 1 + Σ_{s=k}^{m-1} S_s The...
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 91, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *S = NULL;          /* S[0] unused, S[i] = sum rows 1..i */
static seq_t *prefixS = NULL;    /* prefixS[i] = Σ_{j=1}^i S[j], prefixS[0]=0 */
static size_t cap = 0;           /* allocated size minus 1 (idx max)   */
static size_t max_m = 0;         /* highest row index computed so far   */
static seq_t last_rowSum = 0;    /* RowSum of the last computed row   */

/* enlarge the allocation to hold indices up to at least 'need' */
static void grow_to(size_t need)
{
    if (cap >= need)
        return;
    size_t newcap = cap ? cap * 2 : 4;
    while (newcap < need)
        newcap <<= 1;
    if (cap == 0) {
        S = (seq_t *)malloc((newcap + 1) * sizeof(seq_t));
        prefixS = (seq_t *)malloc((newcap + 1) * sizeof(seq_t));
    } else {
        S = (seq_t *)realloc(S, (newcap + 1) * sizeof(seq_t));
        prefixS = (seq_t *)realloc(prefixS, (newcap + 1) * sizeof(seq_t));
    }
    if (!S || !prefixS) {
        /* If allocation fails, leave state unchanged; caller may check for 0. */
        return;
    }
    cap = newcap;
}

/* ensure that S[1..need] and prefixS[1..need] are computed */
static void compute_to(size_t need)
{
    if (max_m >= need)
        return;
    grow_to(need);
    for (size_t i = max_m + 1; i <= need; ++i) {
        if (i == 1) {
            S[1] = (seq_t)1;
            prefixS[1] = S[1];
            last_rowSum = (seq_t)1;   /* RowSum_1 */
        } else {
            seq_t rowSum_i = last_rowSum + (seq_t)(i - 1) * S[i - 1] + (seq_t)1;
            seq_t Si = S[i - 1] + rowSum_i;
            prefixS[i] = prefixS[i - 1] + Si;
            S[i] = Si;
            last_rowSum = rowSum_i;
        }
    }
    max_m = need;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of offset */
    /* Find row m containing term n */
    idx_t m = 1;
    idx_t tri = (idx_t)1;           /* m*(m+1)/2 */
    while (tri < n) {
        ++m;
        tri = (idx_t)m * (m + 1) / 2;
    }
    /* Column k within that row */
    idx_t prev_tri = (idx_t)(m - 1) * m / 2;
    idx_t k = n - prev_tri;          /* 1 ≤ k ≤ m */
    if (k == m)                      /* last element of a row is always 1 */
        return (seq_t)1;

    /* Ensure necessary sums are computed */
    compute_to((size_t)(m - 1));
    if (!S || !prefixS)   /* allocation failure */
        return (seq_t)0;

    /* a(m,k) = 1 + Σ_{s=k}^{m-1} S_s = 1 + prefixS[m-1] - prefixS[k-1]   */
    seq_t val = (seq_t)1 + prefixS[m - 1];
    if (k > 1)
        val -= prefixS[k - 1];
    return val;
}
