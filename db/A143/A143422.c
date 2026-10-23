/*
 * A143422  Number of even numbers k such that phi(k) = n, where n runs
 *          through the values (A002202) taken by phi.
 * https://oeis.org/A143422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even k such that φ(k) = m, where m is the n-th
 *            distinct value of the Euler totient sequence A002202
 * Range:     u8 n <= 7476, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The following fixed-size limit is sufficient for
    n up to about 10000. */
#define MAX_N 12000

#include <stdint.h>
#include <stdlib.h>

/*  seq_t and idx_t are defined by the build system  */
static seq_t *seq_values = NULL;   /* 1‑based array: seq_values[n] = a(n) */
static idx_t  seq_len   = 0;       /* number of values computed (should be >= MAX_N) */

static void precompute(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /*  Start with a reasonable upper bound for totient generation  */
    idx_t limit = 200000;            /* will be increased if needed   */
    while (1) {
        /*  Allocate necessary arrays  */
        idx_t *phi = (idx_t *)calloc(limit + 1, sizeof(idx_t));
        unsigned char *is_composite = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
        unsigned char *seen = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
        seq_t *even_cnt = (seq_t *)calloc(limit + 1, sizeof(seq_t));

        /*  Linear sieve for Euler's totient  */
        idx_t *pr = (idx_t *)malloc((limit/10 + 10) * sizeof(idx_t));
        idx_t pr_cnt = 0, pr_cap = limit/10 + 10;
        phi[1] = 1;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_composite[i]) {
                if (pr_cnt == pr_cap) {
                    pr_cap *= 2;
                    pr = (idx_t *)realloc(pr, pr_cap * sizeof(idx_t));
                }
                pr[pr_cnt++] = i;
                phi[i] = i - 1;
            }
            for (idx_t j = 0; j < pr_cnt; ++j) {
                idx_t p = pr[j];
                idx_t t = i * p;
                if (t > limit) break;
                is_composite[t] = 1;
                if (i % p == 0) {
                    phi[t] = phi[i] * p;
                    break;
                } else {
                    phi[t] = phi[i] * (p - 1);
                }
            }
        }
        free(pr);

        /*  Count even preimages and mark seen totient values  */
        for (idx_t i = 1; i <= limit; ++i) {
            if ((i & 1) == 0) {
                even_cnt[phi[i]]++;
            }
            seen[phi[i]] = 1;
        }

        /*  Collect distinct totient values in ascending order  */
        idx_t *distinct = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
        idx_t distinct_cnt = 0;
        for (idx_t v = 1; v <= limit; ++v) {
            if (seen[v]) {
                distinct[distinct_cnt++] = v;
            }
        }

        if (distinct_cnt >= MAX_N) {
            /*  Prepare the final sequence array  */
            seq_values = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
            seq_len = MAX_N;
            for (idx_t i = 1; i <= MAX_N; ++i) {
                seq_values[i] = even_cnt[distinct[i-1]];
            }

            /*  Free temporary arrays  */
            free(phi);
            free(is_composite);
            free(seen);
            free(even_cnt);
            free(distinct);
            break;          /*  computation complete  */
        }

        /*  Not enough distinct values: increase limit and retry  */
        free(phi);
        free(is_composite);
        free(seen);
        free(even_cnt);
        free(distinct);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    precompute();
    if (n == 0) return 0;               /* offset is 1, but n>=1 in calls */
    if (n > seq_len) return 0;           /* safety: shouldn't happen for given limits */
    return seq_values[n];
}
