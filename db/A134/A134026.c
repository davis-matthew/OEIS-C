/*
 * A134026  Numbers that are in balanced ternary representation longer than in
 *          ternary representation.
 * https://oeis.org/A134026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose balanced ternary representation *
 *            has strictly more digits than its ordinary ternary
 *            representation. * No simple closed formula is known, so we
 *            compute it by brute‑force * enumeration, checking each integer
 *            until the required count is reached. * * Note that the rout...
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the balanced ternary representation of x uses more
 * digits than the ordinary ternary representation.  All arithmetic
 * is performed on seq_t values only. */
static bool qualifies(seq_t x)
{
    /* Ordinary ternary length: number of base‑3 digits. */
    int len3 = 0;
    seq_t t = x;
    while (t > 0) {
        t = t / (seq_t)3;
        len3++;
    }

    /* Balanced ternary length: use the standard digit‑by‑digit method. */
    int lenb = 0;
    t = x;
    while (t > 0) {
        seq_t r = t % (seq_t)3;
        if (r == (seq_t)2) {          // 2 is treated as –1 with a carry
            t = t / (seq_t)3 + (seq_t)1;
        } else {
            t = t / (seq_t)3;
        }
        lenb++;
    }
    return lenb > len3;
}

/* External linkage.  Computes the *n*‑th term of A134026.  The
 * function recomputes from scratch for each call; no caching is
 * required, and therefore it works for any idx_t/seq_t combination. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, unused 0th term */

    idx_t found = 0;        /* how many qualifying numbers found so far */
    idx_t cand  = 1;        /* candidate integer to test (idx_t) */
    seq_t  result = 0;      /* placeholder for the n-th term */

    while (found < n) {
        seq_t x = (seq_t)cand;           /* cast candidate to seq_t */
        if (qualifies(x)) {
            found++;
            if (found == n) {
                result = x;
                break;
            }
        }
        cand++;                           /* advance to next integer */
    }
    return result;
}
