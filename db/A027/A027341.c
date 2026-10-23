/*
 * A027341  Number of partitions of n that do not contain 7 as a part.
 * https://oeis.org/A027341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(n-7) where p(n) is the partition function
 *            A000041(n)
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses Euler's recurrence with generalized pentagonal
   numbers to compute p(k) for all 0 ≤ k ≤ n.  It then returns the
   difference p(n) - p(n-7), treating p(k)=0 for k<0.  The routine
   allocates a temporary array of type seq_t, which is valid both for
   native integer types and for the arbitrary‑precision integer class
   used in the big build.  All arithmetic on seq_t values is performed
   with + and -, which are guaranteed to be defined for all supported
   types.  The array is freed before the function returns.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Allocate array for p[0..n].  env() already provides alloc_seq(). */
    seq_t *p = alloc_seq((size_t)n + 1);
    if (!p) return (seq_t)0;          /* allocation failure → 0 */

    /* Base case */
    p[0] = (seq_t)1;

    /* Compute p(i) for i = 1 .. n using Euler's recurrence. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;         /* sum of terms with positive sign   */
        seq_t neg = (seq_t)0;         /* sum of terms with negative sign   */

        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;   /* generalized pentagonal */
            idx_t g2 = k * (3 * k + 1) / 2;

            if (g1 > i && g2 > i) break;     /* finished the series   */

            if (i >= g1) {
                seq_t val = p[i - g1];
                if (k & 1) pos += val; else neg += val;
            }
            if (i >= g2) {
                seq_t val = p[i - g2];
                if (k & 1) pos += val; else neg += val;
            }
        }
        p[i] = pos - neg;                  /* pos ≥ neg for all i */
    }

    /* a(n) = p(n) – p(n-7)  (treat p(k)=0 for k<0). */
    seq_t result;
    if (n < 7) {
        result = p[n];
    } else {
        result = p[n] - p[n - 7];
    }

    /* Free the temporary memory.  env() already provides free_seq(). */
    free_seq(p);
    return result;
}
