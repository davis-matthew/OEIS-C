/*
 * A044072  Numbers k such that string 2,3 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A search-based implementation – iterate over numbers of form
 *            16*m+11 and keep those where (k-1) does NOT contain the base‑4
 *            substring "23". The returned n-th term (n ≥ 1) is the n‑th such
 *            number
 * Range:     u8 n <= 15, u16 n <= 2911, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑4 representation of x contains the substring 23. */
static bool has23(seq_t x)
{
    if (x < (seq_t)2)           /* need at least two digits */
        return false;

    seq_t prev = x % (seq_t)4;  /* least significant digit   */
    x /= (seq_t)4;

    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)4;
        if (cur == (seq_t)2 && prev == (seq_t)3)
            return true;       /* found 23 */
        prev = cur;
        x /= (seq_t)4;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;        /* number of terms found so far */
    idx_t cand  = 0;        /* m in k = 16*m + 11 */

    while (count < n) {
        seq_t k = ((seq_t)16 * (seq_t)cand) + (seq_t)11;  /* candidate k */
        seq_t km1 = k - (seq_t)1;                         /* k-1 */

        if (!has23(km1)) {           /* accept if k-1 has no "23" */
            ++count;
            if (count == n)
                return k;
        }
        ++cand;                     /* next candidate */
    }
    /* this point is never reached for valid n ≥ 1 */
    return (seq_t)0;
}
