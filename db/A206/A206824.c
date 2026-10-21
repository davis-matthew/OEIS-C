/*
 * A206824  Number of solutions (n,k) of s(k) = s(n) (mod n), where 1 <= k < n
 *          and s(k) = k(k+1)/2.
 * https://oeis.org/A206824
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 1 <= k < n | s(k) ≡ s(n) (mod n) }, where s(k) =
 *            k(k+1)/2. Computed by an O(n) scan
 * Range:     u8 n <= 243, u16 n <= 9442, u32 n <= 9442, u64 n <= 9442,
 *            u128 n <= 9442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined below offset */
    /* s(n) modulo n */
    seq_t s_n_mod = ((seq_t)n * ((seq_t)n + 1) / 2) % (seq_t)n;

    seq_t count = 0;
    seq_t cur = 0;                      /* running sum s(k) modulo n */
    for (idx_t k = 1; k < n; ++k) {
        cur += k;
        cur %= (seq_t)n;                /* keep within modulo */
        if (cur == s_n_mod) ++count;
    }
    return count;
}
