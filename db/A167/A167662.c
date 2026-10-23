/*
 * A167662  Records for partitions into odd squares, cf. A167661.
 * https://oeis.org/A167662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = the n-th record value of p_o(k) where p_o(k) is the
 *            number of * partitions of k into odd squares. Each new record is
 *            the first value * strictly larger than all previous partition
 *            counts. * * This implementation computes p_o(k) incrementally
 *            using the standard * coin‑change DP over the od...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined in the build environment. */
seq_t a(idx_t n)
{
    /* static state: DP array, record list, and list of odd squares */
    static seq_t   *dp      = NULL;   /* dp[i] = p_o(i)   */
    static size_t   dp_cap  = 0;      /* maximum index stored in dp     */
    static seq_t   *records = NULL;   /* list of record values           */
    static size_t   rec_len = 0;      /* number of records stored        */
    static size_t   rec_cap = 0;      /* allocated capacity for records  */
    static seq_t    max_record = (seq_t)0; /* last record value */
    static idx_t   *sqs     = NULL;   /* odd squares used so far          */
    static size_t   sqs_len  = 0;            /* number of squares stored        */
    static size_t   sqs_cap  = 0;            /* capacity of sqs                 */

    /* initialise DP array on first call */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t));   /* capacity 1 for index 0  */
        if (!dp) abort();
        dp[0] = (seq_t)1;                     /* only one way to make 0   */
        dp_cap = 0;
    }

    /* generate records until we have at least n of them */
    while (rec_len < n) {
        /* increase the DP range */
        size_t old_cap = dp_cap;
        size_t new_cap = (dp_cap == 0) ? 256 : dp_cap * 2 + 256;
        /* reallocate DP array */
        seq_t *new_dp = (seq_t *)realloc(dp, (new_cap + 1) * sizeof(seq_t));
        if (!new_dp) abort();
        dp = new_dp;
        for (size_t i = old_cap + 1; i <= new_cap; ++i)
            dp[i] = (seq_t)0;
        dp_cap = new_cap;

        /* update DP for all previously known odd squares */
        for (size_t idx = 0; idx < sqs_len; ++idx) {
            size_t s = (size_t)sqs[idx];
            size_t start = (s > old_cap) ? s : old_cap + 1;
            for (size_t i = start; i <= new_cap; ++i)
                dp[i] += dp[i - s];
        }

        /* generate any new odd squares that fit within the new range */
        while (1) {
            /* next odd square: (2 * sqs_len + 1)^2 */
            size_t k = 2 * sqs_len + 1;
            size_t s = k * k;
            if (s > new_cap) break;

            /* store the new square */
            if (sqs_len == sqs_cap) {
                size_t new_c = sqs_cap ? sqs_cap * 2 : 2;
                idx_t *tmp = (idx_t *)realloc(sqs, new_c * sizeof(idx_t));
                if (!tmp) abort();
                sqs = tmp;
                sqs_cap = new_c;
            }
            sqs[sqs_len++] = (idx_t)s;

            /* update DP for all indices affected by this new square */
            for (size_t i = s; i <= new_cap; ++i)
                dp[i] += dp[i - s];
        }

        /* record any new partition totals that beat the previous maximum */
        for (size_t i = old_cap + 1; i <= new_cap; ++i) {
            if (dp[i] > max_record) {
                max_record = dp[i];
                if (rec_len == rec_cap) {
                    size_t new_c = rec_cap ? rec_cap * 2 : 2;
                    seq_t *tmp = (seq_t *)realloc(records, new_c * sizeof(seq_t));
                    if (!tmp) abort();
                    records = tmp;
                    rec_cap = new_c;
                }
                records[rec_len++] = max_record;
            }
        }
    }

    return records[n - 1];
}
