/*
 * A050296  Maximum cardinality of a strongly triple-free subset of {1, 2,
 *          ..., n}.
 * https://oeis.org/A050296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r≤n, gcd(r,6)=1} ceil(cnt(r,n)/2), * where cnt(r,n) is
 *            the number of integers of the form r·2^a·3^b * that do not
 *            exceed n. This yields the size of a maximum * independent set in
 *            the graph with edges (i,2i) and (i,3i)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t r = 1; r <= n; ++r) {
        if (r % 2ULL == 0ULL || r % 3ULL == 0ULL) continue;   /* r coprime to 6 */
        idx_t cnt = 0;
        idx_t val_a = r;
        while (val_a <= n) {
            idx_t val = val_a;
            while (val <= n) {
                ++cnt;
                if (val > n / 3) break;   /* prevent overflow and stop loop */
                val *= 3;
            }
            if (val_a > n / 2) break;       /* prevent overflow and stop loop */
            val_a *= 2;
        }
        total += (cnt + 1) / 2;              /* ceil(cnt/2) */
    }
    return total;
}
