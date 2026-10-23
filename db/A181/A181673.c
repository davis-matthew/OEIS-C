/*
 * A181673  Decimal expansion of Sum_{n>=1} moebius(n)*(1/10)^n.
 * https://oeis.org/A181673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   floor_div10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A181673 – Decimal expansion of Σ_{n≥1} μ(n) / 10^n
 * Offset: 0 (a(0) is the first term listed above)
 * The returned value a(n) is:
 *   a(0) = integer part   (0 for this series)
 *   a(n) = digit at decimal position n+1, for n ≥ 1
 *
 * The function performs a run‑time computation of the Möbius function up
 * to a fixed upper bound (at least 20000).  The digits are obtained by
 * adding μ(k) to the array of digits and propagating carries using
 * floor‑division by 10.  The result fits into a single decimal digit,
 * which is returned as a seq_t value.
 *
 * The algorithm is O(N log log N) for a maximum N of the order of a few
 * tens of thousands and stays well below the 2.0 s limit.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_div10.h"

/* static buffers for memoisation */
static idx_t   precomp_limit = 0;    /* largest n for which we have data   */
static int8_t *mu_buf        = NULL; /* Möbius values 0 … precomp_limit      */
static int    *dig_buf       = NULL; /* digit array 0 … precomp_limit+1      */

/* ensure precomputation of μ and digits up to at least 'need' (need ≥ 1) */
static void build_buffers(idx_t need)
{
    /* We always precompute at least 20000 to protect against far carry‑outs */
    idx_t target = (need < 20000ULL) ? 20000ULL : need;
    if (target <= precomp_limit) return;          /* already ready */

    /* Allocate new buffers */
    int8_t *new_mu = (int8_t*)malloc((target + 1) * sizeof(int8_t));
    int    *new_dig = (int*)calloc((target + 2), sizeof(int)); /* 0…target+1 */

    /* Linear sieve for Möbius up to 'target' */
    int *spf = (int*)calloc((target + 1), sizeof(int));
    int *pr  = (int*)malloc((target + 1) * sizeof(int));
    int pc = 0;
    new_mu[0] = 0;
    if (target >= 1) new_mu[1] = 1;

    for (int i = 2; i <= (int)target; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            pr[pc++] = i;
        }
        for (int j = 0; j < pc &&
                     pr[j] <= spf[i] &&
                     (long long)i * pr[j] <= target; ++j) {
            int v = i * pr[j];
            spf[v] = pr[j];
        }
    }
    for (int i = 2; i <= (int)target; ++i) {
        int p = spf[i];
        int q = i / p;
        new_mu[i] = (q % p == 0) ? 0 : -new_mu[q];
    }

    /* Add μ values to digit array at their positions */
    for (int i = 1; i <= (int)target + 1; ++i) {
        int val = 0;
        if (i <= (int)target) val = (int)new_mu[i];
        new_dig[i] += val;
    }

    /* Propagate carries from high indices down to 0 */
    for (int i = (int)target + 1; i >= 1; --i) {
        int carry = floor_div10(new_dig[i]);
        new_dig[i] -= carry * 10;
        new_dig[i - 1] += carry;
    }

    /* Replace old buffers */
    free(mu_buf);
    free(dig_buf);
    free(spf);
    free(pr);
    mu_buf  = new_mu;
    dig_buf = new_dig;
    precomp_limit = target;
}

seq_t a(idx_t n)
{
    /* For n=0 we need at least digit 0 ; ensure minimal size 1 */
    if (n == 0) {
        build_buffers(1);
        return (seq_t)dig_buf[0];
    }

    /* We need digits up to index (n+1) (since a(n)=digit_{n+1}) */
    build_buffers((idx_t)n + 1);
    return (seq_t)dig_buf[(int)n + 1];
}
