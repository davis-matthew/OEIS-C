/*
 * A238215  The total number of 1's in all partitions of n into an even number
 *          of distinct parts.
 * https://oeis.org/A238215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of 1's in all partitions of n into an even
 *            number of distinct parts. Implementation uses a
 *            dynamic‑programming recurrence that maintains the aggregate sums
 *            and counts for even and odd cardinality partitions during the
 *            distinct-part sieve
 * Range:     u8 n <= 39, u16 n <= 94, u32 n <= 263, u64 n <= 837,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static arrays storing intermediate DP values */
    static seq_t *even_cnt = NULL, *odd_cnt = NULL;
    static seq_t *even_sum = NULL, *odd_sum = NULL;
    static size_t max_n = 0;          /* largest n for which DP has been built */
    static bool built = false;

    /* If we need to compute up to a larger n, recompute from scratch. */
    if (!built || n > max_n) {
        size_t sz = (size_t)n + 1;
        free(even_cnt); free(odd_cnt); free(even_sum); free(odd_sum);

        /* Allocate zeroed arrays. */
        even_cnt = (seq_t *)calloc(sz, sizeof(seq_t));
        odd_cnt  = (seq_t *)calloc(sz, sizeof(seq_t));
        even_sum = (seq_t *)calloc(sz, sizeof(seq_t));
        odd_sum  = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!even_cnt || !odd_cnt || !even_sum || !odd_sum)
            abort();  /* memory allocation failed */

        /* Base partition: empty set counts as an even‑cardinality partition. */
        even_cnt[0] = 1;

        /* Build partition tables using all parts 1..n. */
        for (idx_t part = 1; part <= n; ++part) {
            /* Iterate over all sums that can currently be formed. */
            if (n >= part) {
                for (idx_t sum = n; ; --sum) {
                    seq_t ce = even_cnt[sum - part];
                    seq_t se = even_sum[sum - part];
                    seq_t co = odd_cnt[sum - part];
                    seq_t so = odd_sum[sum - part];

                    /* Adding 'part' toggles the parity of the number of parts. */
                    /* Resulting partitions now have an odd number of parts. */
                    odd_cnt[sum] += ce;
                    odd_sum[sum] += se + ((part == 1) ? ce : 0);

                    /* Resulting partitions now have an even number of parts. */
                    even_cnt[sum] += co;
                    even_sum[sum] += so + ((part == 1) ? co : 0);

                    if (sum == part) break;  /* avoid unsigned underflow */
                }
            }
        }

        max_n = n;
        built = true;
    }
    return even_sum[n];
}
