/*
 * A090666  For each possible representation of n as n = 2*nb + 3*nu + K with
 *          nb, nu, K nonnegative integers, add numbers K, K+1, ..., 2*K
 *          except for 2*K-1 into a multiset. a(n) is the size of the
 *          resulting multiset minus the number of distinct numbers in it.
 * https://oeis.org/A090666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over representations of (size of the L‑set) minus the
 *            number of distinct L values. Compute by enumerating the possible
 *            K values, counting the admissible ν values, accumulating the
 *            total multiplicity, and marking all distinct L values that
 *            appear
 * Range:     u8 n <= 19, u16 n <= 130, u32 n <= 671, u64 n <= 671,
 *            u128 n <= 671, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* maximum L is 2*n, so we need an array for 0 … 2*n */
    size_t visited_size = 2 * (size_t)n + 1;
    unsigned char *visited = (unsigned char *)calloc(visited_size, 1);
    if (!visited) return (seq_t)0;   /* out‑of‑memory fallback; result 0 */

    seq_t total_mult  = 0;   /* M – total multiplicity of L values */
    seq_t distinct_cnt = 0;  /* D – number of distinct L values */

    for (idx_t K = 0; K <= n; ++K) {
        idx_t R = n - K;          /* remaining part to be expressed by 2*nb+3*nu */
        idx_t max_nu = R / 3;     /* maximum ν such that 3*ν ≤ R */
        idx_t start_nu = R & 1;   /* first ν with parity matching R (ν ≡ R (mod 2)) */
        idx_t reps = 0;
        if (max_nu >= start_nu) {
            reps = (max_nu - start_nu) / 2 + 1;  /* count of admissible ν */
        }
        if (reps == 0) continue;   /* no representation with this K – skip L set */

        seq_t lcount = (K == 0) ? (seq_t)1 : (seq_t)K;   /* size of the L‑set for this K */
        total_mult += (seq_t)reps * lcount;

        idx_t twoK = K + K;
        for (idx_t L = K; L <= twoK; ++L) {
            if (L == twoK - 1) continue;  /* exclude 2*K-1 */
            if (!visited[(size_t)L]) {
                visited[(size_t)L] = 1;
                ++distinct_cnt;
            }
        }
    }

    free(visited);
    return total_mult - distinct_cnt;
}
