/*
 * A124330  a(n)= ((d(n) mod phi(n)) +1)th positive integer which is coprime
 *          to n, where phi(n) is number of positive integers which are <= n
 *          and are coprime to n and d(n) is the number of positive divisors
 *          of n.
 * https://oeis.org/A124330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 9294, u32 n <= 9294, u64 n <= 9294,
 *            u128 n <= 9294, big any n
 * Helpers:   factor_and_phi, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A124330: a(n) = the ((d(n) mod φ(n)) + 1)-th positive integer
   that is coprime to n.  d(n) = number of positive divisors of n,
   φ(n) = Euler's totient of n.  The function works for any
   unsigned integer type seq_t (including arbitrary‐precision) and
   any index type idx_t (uint64_t).  It uses trial division for
   factoring n; n ≤ 10 000 is fast, but larger arguments are still
   feasible with a small performance penalty. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"
#include "../utils/factor_and_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* safety – sequence is defined for n≥1 */

    /* Factor n and compute d(n) and φ(n). */
    seq_t d, phi;
    factor_and_phi(n, &d, &phi);

    /* Compute ((d mod φ) + 1)-th coprime. */
    seq_t r = d % phi;          /* 0 ≤ r < φ */
    seq_t k = r + (seq_t)1;     /* 1‑based index */

    /* Search for the k-th coprime number ≤ n. */
    idx_t count = 0;
    seq_t ans   = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        if (seq_gcd((seq_t)i, (seq_t)n) == (seq_t)1) {
            ++count;
            if (count == k) {
                ans = (seq_t)i;
                break;
            }
        }
    }
    return ans;
}
