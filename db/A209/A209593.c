/*
 * A209593  T(n,k)=Number of n X n 0..k arrays with every element equal to a
 *          diagonal or antidiagonal reflection.
 * https://oeis.org/A209593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (k+1)^(2n-(n mod 2)) * ((k+1)*(2k+1))^((n*n-2n+(n mod
 *            2))/4) The sequence a(n) enumerates all pairs (n,k) (1‑based) in
 *            order of increasing sum n+k, and for each fixed sum in
 *            decreasing k order (i.e. k = sum‑1, sum‑2, …, 1). The term a(i)
 *            equals T(n,k) for the i‑th such pair
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 42, u128 n <= 75,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

/* Convert a global sequence index to the corresponding (n,k) pair.  */
static void index_to_pair(idx_t idx, idx_t *pn, idx_t *pk)
{
    /* Find minimal m such that (m-1)*m/2 >= idx  */
    idx_t low = 2, high = idx + 1;          /* high is guaranteed large enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)(mid - 1) * mid / 2;
        if (tri >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                         /* m = n + k  */

    __int128 prev_tri = (__int128)(m - 2) * (m - 1) / 2;
    idx_t offset = idx - (idx_t)prev_tri;   /* offset = 1 … (m-1)  */

    *pn = offset;                     /* n = offset            */
    *pk = m - offset;                 /* k = m - offset        */
}

/* Main sequence function.  */
seq_t a(idx_t idx)
{
    idx_t n, k;
    index_to_pair(idx, &n, &k);

    /* Exponents as 32‑bit signed integers;   */
    int exp1 = (int)((2 * n) - (n & 1));
    int exp2 = (int)((n * n - 2 * n + (n & 1)) / 4);

    seq_t base1 = (seq_t)(k + 1);
    seq_t base2 = base1 * ((seq_t)2 * k + 1);

    seq_t t1 = pow_seq(base1, exp1);
    seq_t t2 = pow_seq(base2, exp2);

    return t1 * t2;
}
