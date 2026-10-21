/*
 * A222709  Total number of parts of multiplicity 9 in all partitions of n.
 * https://oeis.org/A222709
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m>=1} (p(n-9m)-p(n-10m)) for n>=9, where p(k) is the
 *            unrestricted partition number (p(k)=0 for k<0). For n<9, a(n)=0
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 150, u64 n <= 445,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 9) return 0;

    static seq_t *part = NULL;   /* partition numbers p[0..max_n]   */
    static idx_t  max_n = 0;     /* highest n for which array is ready */

    /* Grow or create the partition array if needed */
    if (n > max_n) {
        seq_t *new_part = alloc_seq_array(n + 1);
        for (idx_t i = 0; i <= n; ++i) new_part[i] = 0;
        new_part[0] = 1;                 /* p(0) = 1 */

        /* Compute all partition numbers up to n using the
           simple coin‑change DP (partition generating function).  */
        for (idx_t k = 1; k <= n; ++k) {
            for (idx_t j = k; j <= n; ++j) {
                new_part[j] += new_part[j - k];
            }
        }

        /* Replace old array (if any) and update the maximum size. */
        if (part != NULL) free_seq_array(part);
        part   = new_part;
        max_n  = n;
    }

    /* Compute a(n) from the stored partition numbers. */
    seq_t total = 0;
    for (idx_t m = 1; ; ++m) {
        idx_t t9  = 9 * m;
        if (t9 > n) break;
        idx_t t10 = 10 * m;

        seq_t term = part[n - t9];
        if (t10 <= n) term -= part[n - t10];  /* no underflow: p[n-9m] >= p[n-10m] */

        total += term;
    }
    return total;
}
