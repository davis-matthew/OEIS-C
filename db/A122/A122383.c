/*
 * A122383  a(n) = m-th positive integer which is coprime to n, where phi(n)
 *          is number of positive integers which are <= n and are coprime to n
 *          and m = phi(n) if phi(n)|n, else m = n mod phi(n).
 * https://oeis.org/A122383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the m‑th positive integer coprime to n, where φ(n) is the
 *            count of 1 ≤ k ≤ n with gcd(k,n)=1, and m = φ(n) if φ(n) divides
 *            n, otherwise m = n mod φ(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) by simple scanning – n ≤ 10000, fast enough */
    idx_t phi = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd((seq_t)n, (seq_t)i) == 1) {
            ++phi;
        }
    }

    /* Determine m */
    idx_t m = (n % phi == 0) ? phi : n % phi;

    /* Find the m‑th coprime residue modulo n */
    idx_t cnt = 0, resid = 1;
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd((seq_t)n, (seq_t)i) == 1) {
            ++cnt;
            if (cnt == m) {
                resid = i;
                break;
            }
        }
    }

    /* Compute the corresponding positive integer */
    idx_t q = (m - 1) / phi;          /* how many full periods before */
    seq_t result = (seq_t)(q * n + resid);
    return result;
}
