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
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205850:
   a(n) = (F_{k+1} - F_{j+1}) / 4,
   where (k, j) are the integer pairs produced by A205847 and A205848;
   they are listed by increasing k, then increasing j within each k,
   and include only those pairs for which F_{k+1} ≡ F_{j+1} (mod 4). */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Group identifier for k based on F_{k+1} (mod 4):
   0 → F ≡ 1 (mod 4)  (indices with (k+1) mod 6 = 1, 2, 5)
   1 → F ≡ 2 (mod 4)  (indices with (k+1) mod 6 = 3)
   2 → F ≡ 3 (mod 4)  (indices with (k+1) mod 6 = 4)
   3 → F ≡ 0 (mod 4)  (indices with (k+1) mod 6 = 0) */
static inline int group_of(idx_t idx)
{
    unsigned r = (unsigned)((idx + 1ULL) % 6ULL);
    if (r == 0) return 3;   /* F ≡ 0 (mod 4)  */
    if (r == 3) return 1;   /* F ≡ 2 (mod 4)  */
    if (r == 4) return 2;   /* F ≡ 3 (mod 4)  */
    return 0;               /* F ≡ 1 (mod 4)  */
}

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
