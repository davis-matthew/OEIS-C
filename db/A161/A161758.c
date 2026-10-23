/*
 * A161758  a(n)=n-p+1 where p is the maximal prime less than n-2.
 * https://oeis.org/A161758
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - p + 1 where p is the maximal prime less than n-2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* simple primality test for idx_t numbers (valid for n up to ~10^7) */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Offset starts at n=5; behaviour for n<5 is irrelevant */
    if (n < 5) return 0;

    /* Find the largest prime < n-2  → start from n-3 and search downward */
    idx_t start = n - 3;
    idx_t p = 2;  /* default in case we never find a larger prime */

    for (idx_t k = start; ; --k) {
        if (is_prime_idx(k)) { p = k; break; }
        if (k == 2) { p = 2; break; } /* stop when we hit the smallest prime */
    }

    return (seq_t)(n - p + 1);
}
