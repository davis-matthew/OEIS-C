/*
 * A222178  Numbers n such that 8n is a partition number.
 * https://oeis.org/A222178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(k)/8 for all k such that the partition number p(k) is
 *            divisible by 8. Here p(k) is the number of partitions of k. The
 *            sequence lists the quotients of such partition numbers
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Terminals:  p(k) stored as 128‑bit to avoid overflow during
       the calculation.  The final quotient is cast down to seq_t.  */
    static __uint128_t *part_arr = NULL;   /* p(0), p(1), ...   */
    static size_t part_cap = 0, part_len = 0; /* current capacity and length */

    /* Arrays holding the computed terms of A222178. */
    static seq_t *term_arr = NULL;
    static size_t term_cap = 0, term_len = 0;

    /* Offset is 1, so a(1) is the first term.  The specification
       states that behaviour for indices < 1 is irrelevant, but we
       return 0 for safety. */
    if (n == 0) return 0;

    /* Initialise storage if required. */
    if (!part_arr) {
        part_cap = 64;
        part_arr = (__uint128_t *)malloc(part_cap * sizeof(__uint128_t));
        part_arr[0] = 1;          /* p(0) = 1 */
        part_len = 1;
    }
    if (!term_arr) {
        term_cap = 8;
        term_arr = (seq_t *)malloc(term_cap * sizeof(seq_t));
        term_len = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (term_len < n) {
        /* Compute the next partition number p(k) where k == part_len. */
        idx_t k = part_len;

        if (part_len >= part_cap) {
            part_cap = part_cap * 2;
            part_arr = (__uint128_t *)realloc(part_arr, part_cap * sizeof(__uint128_t));
        }

        __uint128_t pos = 0, neg = 0;
        for (idx_t t = 1; ; ++t) {
            idx_t pent1 = t * (3 * t - 1) / 2;
            idx_t pent2 = t * (3 * t + 1) / 2;
            if (pent1 > k && pent2 > k) break;

            __uint128_t val = 0;
            if (pent1 <= k) val += part_arr[k - pent1];
            if (pent2 <= k) val += part_arr[k - pent2];

            /* sign is (+1) for odd t, (-1) for even t. */
            if (t & 1) pos += val;
            else       neg += val;
        }

        __uint128_t pk = pos - neg;
        part_arr[part_len++] = pk;

        /* Check if the partition number is divisible by 8. */
        if ((pk & 7) == 0) {
            __uint128_t quo = pk >> 3;        /* pk / 8 */
            /* Store only if the quotient fits in seq_t. */
            seq_t max_seq = (seq_t)-1;
            if (quo <= ( __uint128_t)max_seq) {
                if (term_len >= term_cap) {
                    term_cap = term_cap * 2;
                    term_arr = (seq_t *)realloc(term_arr, term_cap * sizeof(seq_t));
                }
                term_arr[term_len++] = (seq_t)quo;
            }
        }
    }

    return term_arr[n - 1];
}
