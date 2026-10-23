/*
 * A158973  a(n) = count of numbers k <= n such that all proper divisors of k
 *          are divisors of n.
 * https://oeis.org/A158973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of k ≤ n such that every proper divisor of k divides n.
 *            Implementation: brute‑force enumeration of all divisors of each
 *            k
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1, but guard just in case     */
    seq_t count = 0;              /* use seq_t for the result              */

    for (idx_t k = 1; k <= n; ++k) {
        int ok = 1;               /* true until a counterexample is found */

        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                if (d != k) {
                    if (n % d != 0) { ok = 0; break; }
                }
                idx_t e = k / d;   /* the complementary divisor          */
                if (e != d && e != k) {
                    if (n % e != 0) { ok = 0; break; }
                }
            }
        }

        if (ok) ++count;
    }

    return count;
}
