/*
 * A174996  Triangle read by rows: T(n,k) = (prime(n)-1) mod prime(k).
 * https://oeis.org/A174996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS A174996: triangle read by rows
   T(m,k) = (prime(m)-1) mod prime(k), where m≥1, 1≤k≤m.
   The sequence a(n) enumerates the triangle row‑by‑row,
   so a(n) is T(m,k) with m the row containing the n‑th term
   and k the position within that row. */
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* offset is 1, below values undefined */

    /* Find the row m such that m*(m+1)/2 ≥ n.
       The cumulative count up to row m-1 is (m-1)*m/2. */
    idx_t m = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++m;
        cumulative += m;
    }

    /* Column index k within row m */
    idx_t k = n - (cumulative - m);   /* 1 ≤ k ≤ m */

    /* Compute (prime(m)-1) mod prime(k) */
    uint32_t pm = get_prime(m);
    uint32_t pk = get_prime(k);

    seq_t result = (seq_t)(pm - 1) % (seq_t)pk;
    return result;
}
