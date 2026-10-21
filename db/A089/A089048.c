/*
 * A089048  Number of ways of writing n as a sum of exactly 3 powers of 2.
 * https://oeis.org/A089048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered triples (i,j,k), 0 ≤ i ≤ j ≤ k, with
 *            2^i + 2^j + 2^k = n. Powers may be equal. The algorithm runs in
 *            O((log n)^3) time by iterating over the exponents of the largest
 *            power and testing all pairs for the remaining two powers. All
 *            arithmetic that may overflow a 64‑bit valu...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n){
    /* Pre‑compute powers of two once. */
    static int built = 0;
    static seq_t pow2[64];
    if (!built){
        pow2[0] = (seq_t)1;
        for (int e = 1; e < 64; ++e)
            pow2[e] = pow2[e-1] + pow2[e-1];
        built = 1;
    }

    if (n < 3) return (seq_t)0;

    seq_t count = (seq_t)0;

    /* Compute maximum exponent k such that 2^k ≤ n. */
    idx_t kmax = 0;
    idx_t tmp = n;
    while ((tmp >> 1) != 0){
        tmp >>= 1;
        ++kmax;
    }

    const unsigned __int128 N = (unsigned __int128)n;

    for (idx_t k = 0; k <= kmax; ++k){
        seq_t two_k = pow2[k];
        if (two_k > n) continue;

        for (idx_t i = 0; i <= k; ++i){
            seq_t two_i = pow2[i];
            if (two_i > n) break;

            for (idx_t j = i; j <= k; ++j){
                unsigned __int128 sum = (unsigned __int128)two_i
                                      + (unsigned __int128)pow2[j]
                                      + (unsigned __int128)two_k;
                if (sum == N) ++count;
                else if (sum > N) break; /* further j increase sum */
            }
        }
    }
    return count;
}
