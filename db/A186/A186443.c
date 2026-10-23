/*
 * A186443  Records in A087029.
 * https://oeis.org/A186443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=9, a(2)=18, a(3)=10*a(1); for n≥4: if n is even: a(n) =
 *            2*a(n-1); if n is odd and n≥5: a(n) = 9*a(n-2) + a(n-4); The
 *            sequence A186443 is thus defined by this simple recurrence
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation storage – 1‑based indices, index 0 unused */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;   /* allocated capacity (n+1) */
    static idx_t seq_len  = 0;   /* largest index computed so far */

    /* grow storage if needed */
    if (n >= seq_cap) {
        size_t new_cap = (n + 1) * 2;   /* double capacity for amortised O(1) */
        if (new_cap < seq_cap * 2) new_cap = seq_cap * 2;
        if (new_cap == 0) new_cap = n + 1;      /* first allocation */
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* initialise with base terms if not already present */
    if (seq_len < 1) {
        seq[1] = (seq_t)9;
        seq_len = 1;
    }
    if (seq_len < 2) {
        seq[2] = (seq_t)18;
        seq_len = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        if ((i & 1) == 0) {               /* even index */
            seq[i] = (seq_t)2 * seq[i-1];
        } else {                          /* odd index */
            if (i == 3) {
                seq[i] = (seq_t)10 * seq[1];
            } else { /* i >= 5 */
                seq[i] = (seq_t)9 * seq[i-2] + seq[i-4];
            }
        }
    }
    seq_len = n;

    return seq[n];
}
