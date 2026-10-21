/*
 * A091478  Table of graphs with n (>=0) nodes and k (>=0) edges. Each type of
 *          object labeled from its own label set.
 * https://oeis.org/A091478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = k! * binomial(binomial(r,2), k) where the index n chooses
 *            r (number of nodes) and k (number of edges) in the flattened
 *            table (row‑major order)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r (number of nodes) and the position k (edge count) corresponding to the linear index n. */
    uint64_t cumulative = 0;
    uint64_t r = 0;
    uint64_t edges = 0;
    uint64_t k = 0;

    while (1) {
        edges = r * (r - 1) / 2;          /* total possible edges for r nodes */
        uint64_t row_size = edges + 1;    /* entries in this row (k = 0 … edges) */

        if (n < cumulative + row_size) {
            k = n - cumulative;
            break;
        }
        cumulative += row_size;
        ++r;
    }

    /* Compute k! */
    seq_t fact = 1;
    for (uint64_t i = 2; i <= k; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute binomial(edges, k) using the multiplicative formula
       (ans *= (edges - k + i) / i) to keep intermediate values integral. */
    seq_t bin = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        bin *= (seq_t)(edges - k + i);
        bin /= (seq_t)i;
    }

    return fact * bin;
}
