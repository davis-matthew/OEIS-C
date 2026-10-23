/*
 * A140180  A number n is included if the smallest prime that is congruent to
 *          1 (mod n) is <= the smallest prime that is congruent to -1 (mod
 *          n).
 * https://oeis.org/A140180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 567, u32 n <= 567, u64 n <= 567,
 *            u128 n <= 567, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A140180: a(n) is the nth positive integer k such that
   the smallest prime ≡ 1 (mod k) is ≤ the smallest prime ≡ -1 (mod k).
   The function scans integers k=1,2,3,…, testing the property and
   returns the nth one.
*/
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n) */

static seq_t first_prime_residue(idx_t n, idx_t r)
{
    /* Special case when n==1: every prime is ≡ 0 (mod 1), so return 2 */
    if (n == 1) return (seq_t)2;

    seq_t cand = (seq_t)r;
    /* Ensure the candidate is at least 2 and congruent to r mod n */
    if (cand < (seq_t)2) cand += (seq_t)n;

    /* Step by n to stay in the same residue class */
    const seq_t step = (seq_t)n;
    while (1) {
        if (is_prime_big(cand)) return cand;
        cand += step;
    }
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 1;

    while (1) {
        bool ok;
        if (k == 1) {
            ok = true;                     /* 1 always satisfies the property */
        } else {
            seq_t p1    = first_prime_residue(k, 1);          /* 1 (mod k)  */
            seq_t p_minus = first_prime_residue(k, k - 1);    /* -1 (mod k) */
            ok = (p1 <= p_minus);
        }

        if (ok) {
            found++;
            if (found == n)
                return (seq_t)k;
        }
        k++;
    }
}
