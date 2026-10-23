/*
 * A165275  Table read by antidiagonals: T(n, k) is the k-th number with n-1
 *          odd-power summands in its base 2 representation.
 * https://oeis.org/A165275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   count_len_with_limit, count_lower_with_limit, unrank_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A165275 – Table read by antidiagonals.
 * a(n) = k-th element of the antidiagonal enumeration of the array
 * T(n,k) = the k-th positive integer with n‑1 odd‑power summands in its
 *          base‑2 representation.
 * The implementation uses combinatorial ranking of binary strings
 * with a fixed number of odd positions set.
 * No external data structures or huge tables are required – the
 * algorithm runs in O(n·r) time where r ≤ 200 for n ≤ 10000.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_len_with_limit.h"
#include "../utils/count_lower_with_limit.h"
#include "../utils/unrank_row.h"

/* Public entry point.  n is 1‑based (OEIS offset). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* defensive: OEIS offset starts at 1 */

    /* Convert 1‑based index to 0‑based linear index. */
    idx_t i = n - 1;

    /* Find diagonal sum s such that triangular(s) <= i < triangular(s+1). */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= i) {
        ++s;
    }

    /* Determine row (number of odd bits) and column (rank within that row). */
    idx_t d = i - (s * (s + 1)) / 2;
    idx_t r = d;              /* row index: r == number of odd powers */
    idx_t c = s - d;          /* column index: c == rank within row r */

    seq_t rank = (seq_t)c;
    return unrank_row(rank, r);
}
