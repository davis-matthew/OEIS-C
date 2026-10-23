/*
 * A047715  Numbers that are the sum of 4 but no fewer nonzero fourth powers.
 * https://oeis.org/A047715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047715: numbers that are the sum of 4 but no fewer nonzero fourth powers. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for computed terms */
static seq_t *seq = NULL;
static idx_t seq_len = 0;      /* number of terms stored */
static idx_t cur_limit = 0;    /* current upper bound used for computation */

#define INITIAL_LIMIT 400000ULL

/* Recompute the sequence until at least n terms are available */
static void ensure_terms(idx_t n)
{
    if (n <= seq_len)
        return;

    if (cur_limit == 0)
        cur_limit = INITIAL_LIMIT;

    while (true) {
        /* Boolean arrays for sums of 1–4 fourth powers */
        uint8_t *s1 = (uint8_t*)calloc(cur_limit + 1, 1);
        uint8_t *s2 = (uint8_t*)calloc(cur_limit + 1, 1);
        uint8_t *s3 = (uint8_t*)calloc(cur_limit + 1, 1);
        uint8_t *s4 = (uint8_t*)calloc(cur_limit + 1, 1);
        if (!s1 || !s2 || !s3 || !s4)
            return; /* memory error, give up */

        /* Find maximal integer m with m^4 <= cur_limit */
        idx_t maxM = 1;
        seq_t lim = (seq_t)cur_limit;
        while (true) {
            seq_t v = (seq_t)(maxM + 1) * (maxM + 1) * (maxM + 1) * (maxM + 1);
            if (v > lim)
                break;
            ++maxM;
        }

        /* Precompute fourth powers */
        seq_t *pow4 = (seq_t*)malloc((size_t)(maxM + 1) * sizeof(seq_t));
        if (!pow4) {
            free(s1); free(s2); free(s3); free(s4);
            return;
        }
        for (idx_t i = 1; i <= maxM; ++i)
            pow4[i] = (seq_t)i * i * i * i;

        /* Enumerate sums of 1, 2, 3, and 4 fourth powers */
        for (idx_t a = 1; a <= maxM; ++a) {
            seq_t va = pow4[a];
            idx_t ia = (idx_t)va;
            if (ia <= cur_limit)
                s1[ia] = 1;
            else
                continue; /* va > cur_limit, no need to proceed with this a */

            for (idx_t b = a; b <= maxM; ++b) {
                seq_t vb = pow4[b];
                seq_t sb = va + vb;
                idx_t isb = (idx_t)sb;
                if (isb <= cur_limit)
                    s2[isb] = 1;
                else
                    continue; /* sb > cur_limit */

                for (idx_t c = b; c <= maxM; ++c) {
                    seq_t vc = pow4[c];
                    seq_t sc = sb + vc;
                    idx_t isc = (idx_t)sc;
                    if (isc <= cur_limit)
                        s3[isc] = 1;

                    for (idx_t d = c; d <= maxM; ++d) {
                        seq_t vd = pow4[d];
                        seq_t sd = sc + vd;
                        idx_t isd = (idx_t)sd;
                        if (isd <= cur_limit)
                            s4[isd] = 1;
                    }
                }
            }
        }

        free(pow4);

        /* Count values obtainable by 4 fourth powers but not by 1–3 */
        idx_t count = 0;
        for (idx_t i = 0; i <= cur_limit; ++i) {
            if (s4[i] && !s1[i] && !s2[i] && !s3[i])
                ++count;
        }

        if (count >= n) {
            seq_t *new_seq = (seq_t*)malloc((size_t)count * sizeof(seq_t));
            if (!new_seq) {
                free(s1); free(s2); free(s3); free(s4);
                return;
            }

            idx_t pos = 0;
            for (idx_t i = 0; i <= cur_limit; ++i) {
                if (s4[i] && !s1[i] && !s2[i] && !s3[i])
                    new_seq[pos++] = (seq_t)i;
            }

            free(seq);
            seq = new_seq;
            seq_len = count;

            free(s1); free(s2); free(s3); free(s4);
            break; /* desired number of terms obtained */
        }

        /* Not enough terms: double the bound and retry */
        free(s1); free(s2); free(s3); free(s4);
        cur_limit *= 2ULL;
    }
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0; return 0 to avoid UB */

    ensure_terms(n);            /* make sure at least n terms are computed */
    return seq[n - 1];           /* sequence is 0‑based in the array */
}
