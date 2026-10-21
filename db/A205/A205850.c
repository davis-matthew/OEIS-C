/*
 * A205850  [s(k)-s(j)]/4, where the pairs (k,j) are given by A205847 and
 *          A205848, and s(k) denotes the (k+1)-st Fibonacci number.
 * https://oeis.org/A205850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   fib, group_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205850:
   a(n) = (F_{k+1} - F_{j+1}) / 4,
   where (k, j) are the integer pairs produced by A205847 and A205848;
   they are listed by increasing k, then increasing j within each k,
   and include only those pairs for which F_{k+1} ≡ F_{j+1} (mod 4). */

#include "../utils/fib.h"
#include "../utils/group_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t fk = fib(k + 1);
        int gk = group_of(k);
        for (idx_t j = 1; j < k; ++j) {
            if (group_of(j) == gk) {
                seq_t val = (fk - fib(j + 1)) / 4;
                ++count;
                if (count == n) return val;
            }
        }
    }
}
