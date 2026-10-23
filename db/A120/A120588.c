/*
 * A120588  G.f. is 1 + x*c(x), where c(x) is the g.f. of the Catalan numbers
 *          (A000108).
 * https://oeis.org/A120588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = Catalan(n-1) for n >= 1 Catalan(k) =
 *            binom(2k,k)/(k+1) (computed iteratively)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A120588 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n - 1;                 /* we need Catalan(k) */
    /* Fast path for 64‑bit integers (and smaller).  Here we use
     * unsigned __int128 to avoid overflow during the intermediate step. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned long long cur64 = 1ULL;
        for (idx_t i = 0; i < k; ++i) {
            unsigned __int128 prod = (unsigned __int128)cur64 * (4 * i + 2);
            prod /= (i + 2);
            cur64 = (unsigned long long)prod;
        }
        return (seq_t)cur64;
    }

    /* Generic path for arbitrary‑precision seq_t types.  The operations
     * ( *, / ) are assumed to be defined for the type. */
    seq_t cur = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        seq_t num = (seq_t)(4 * i + 2);
        seq_t den = (seq_t)(i + 2);
        cur = (cur * num) / den;
    }
    return cur;
}
