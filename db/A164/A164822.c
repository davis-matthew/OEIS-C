/*
 * A164822  Triangle read by rows, giving the number of solutions mod j of
 *          T_k(x) = 1, for j >= 2 and k = 1:j-1, where T_k is the k'th
 *          Chebyshev polynomial of the first kind.
 * https://oeis.org/A164822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) counts solutions x (1 ≤ x < j) to T_k(x) ≡ 1 (mod j) where
 *            n maps to the pair (k,j) by reading the triangle by rows: for
 *            each modulus j ≥ 2, list k = 1…j−1
 * Range:     u8 n <= 4950, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* count of x (1 ≤ x < j) such that Chebyshev T_k(x) ≡ 1 (mod j) */
static seq_t count_solutions(idx_t k, idx_t j)
{
    uint64_t j64 = (uint64_t)j;
    uint64_t count = 0;

    for (uint64_t x = 1; x < j64; ++x) {
        if (k == 1) {
            if (x == 1) ++count;
            continue;
        }
        uint64_t t0 = 1 % j64;          /* T_0 */
        uint64_t t1 = x % j64;          /* T_1 */

        for (idx_t idx = 2; idx <= k; ++idx) {
            uint64_t tmp = (uint64_t)((unsigned __int128)2 * x % j64 * t1 % j64);
            uint64_t t2  = (tmp + j64 - t0) % j64;  /* T_idx = 2*x*T_{idx-1} - T_{idx-2} (mod j) */
            t0 = t1;
            t1 = t2;
        }

        if (t1 == 1) ++count;
    }

    return (seq_t)count;
}

seq_t a(idx_t n)
{
    /* find the modulus j that contains the n-th entry */
    idx_t j = 2;
    idx_t cum = 0;          /* total elements before the current row */

    while (true) {
        idx_t row = j - 1;  /* number of entries in this row */
        if (cum + row >= n) break;
        cum += row;
        ++j;
    }

    idx_t k = n - cum;       /* k ranges from 1 to j-1 */

    return count_solutions(k, j);
}
