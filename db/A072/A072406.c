/*
 * A072406  Number of values of k for which C(n,k)-C(n-2,k-1) is odd.
 * https://oeis.org/A072406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of k (0 ≤ k ≤ n) for which C(n,k) –
 *            C(n‑2,k‑1) is odd. Since subtraction modulo 2 is equivalent to
 *            XOR, a(n) counts the k for which exactly one of C(n,k) and
 *            C(n‑2,k‑1) is odd. Using Lucas’ theorem for p = 2, C(n,k) is odd
 *            iff every bit of k appears among the bits of n; i.e. (k...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    const idx_t notn = ~n;           /* complement of n (unsigned) */
    const bool n_ge_2 = (n >= 2);
    const idx_t n2 = n_ge_2 ? n - 2 : 0;
    const idx_t notn2 = n_ge_2 ? ~n2 : 0;   /* only used when n ≥ 2 */

    for (idx_t k = 0; k <= n; ++k) {
        /* parity of C(n,k) */
        const bool odd1 = ((k & notn) == 0);

        /* parity of C(n‑2,k‑1) – defined only when k ≥ 1 and n ≥ 2 */
        bool odd2 = false;
        if (n_ge_2 && k >= 1) {
            const idx_t kp = k - 1;
            odd2 = ((kp & notn2) == 0);
        }

        if (odd1 ^ odd2) {
            ++count;
        }
    }
    return count;
}
