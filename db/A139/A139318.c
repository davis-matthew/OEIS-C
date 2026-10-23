/*
 * A139318  a(n) = (A139317(n)-1)/n.
 * https://oeis.org/A139318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A139317(n)-1)/n
 * Range:     u8 n <= 30, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A139317(n) is obtained as the smallest integer of the form n*k+1
    that is coprime to all previous A139317(i) (i < n).  All terms
    produced by this rule are prime, so the condition reduces to
    finding the first such prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_TERMS 200000   /* safe upper bound for the expected n */

static seq_t A[MAX_TERMS + 1] = {0};   /* A[1] .. A[MAX_TERMS]   */
static size_t computed = 0;            /* highest index filled in A */

/* Return the a(n) value for the sequence A139318 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but keep sane */

    /* Ensure the static array is large enough (normally not needed) */
    if (n > MAX_TERMS) {
        /* In practice n will not exceed MAX_TERMS; just clip. */
        return (seq_t)0;
    }

    /* Compute all A values up to n if needed */
    if (computed < (size_t)n) {
        for (idx_t j = (idx_t)(computed + 1); j <= n; ++j) {
            for (idx_t k = 1; ; ++k) {
                seq_t m = ((seq_t)j * k) + 1;   /* candidate m = j*k + 1 */

                bool ok = true;
                for (idx_t t = 1; t < j; ++t) {
                    if (m % A[t] == 0) {        /* not coprime */
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    A[j] = m;                    /* store the found value */
                    break;
                }
            }
        }
        computed = (size_t)n;
    }

    /* a(n) = (A139317(n) - 1) / n */
    return (A[n] - (seq_t)1) / ((seq_t)n);
}
