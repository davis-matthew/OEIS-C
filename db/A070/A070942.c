/*
 * A070942  Values of n such that the first n terms of A055265 constitute a
 *          permutation of {1,2,3,...,n}.
 * https://oeis.org/A070942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that max(A055265(1..m)) = m; computed by
 *            generating A055265 (a(1)=1, then least unused k with k+previous
 *            prime) and scanning prefixes
 * Range:     u8 n <= 29, u16 n <= 762, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Validated: standard, pass, 74 terms through n = 1458 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *p_comp = NULL;   /* 1 = composite or <2 */
static uint64_t p_lim = 0;
static unsigned char *p_used = NULL;
static uint64_t u_lim = 0;
static uint64_t *p_recs = NULL;
static size_t p_nrec = 0, p_caprec = 0;
static uint64_t p_count = 0, p_max = 0, p_prev = 0, p_minun = 1;

static void p_ensure_sieve(uint64_t need)
{
    if (need <= p_lim) return;
    uint64_t nl = p_lim ? p_lim : 1024;
    while (nl < need) nl *= 2;
    free(p_comp);
    p_comp = (unsigned char *)calloc((size_t)(nl + 1), 1);
    p_comp[0] = 1;
    if (nl >= 1) p_comp[1] = 1;
    for (uint64_t i = 2; i * i <= nl; i++)
        if (!p_comp[i])
            for (uint64_t j = i * i; j <= nl; j += i) p_comp[j] = 1;
    p_lim = nl;
}

static void p_ensure_used(uint64_t need)
{
    if (need <= u_lim) return;
    uint64_t nl = u_lim ? u_lim : 1024;
    while (nl < need) nl *= 2;
    p_used = (unsigned char *)realloc(p_used, (size_t)(nl + 1));
    memset(p_used + u_lim + (u_lim ? 1 : 0), 0, (size_t)(nl - u_lim - (u_lim ? 0 : -1)));
    u_lim = nl;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (p_nrec < (size_t)n) {
        uint64_t t;
        if (p_count == 0) {
            t = 1;
        } else {
            t = p_minun;
            for (;;) {
                p_ensure_used(t + 1);
                p_ensure_sieve(p_prev + t + 1);
                if (!p_used[t] && !p_comp[p_prev + t]) break;
                t++;
            }
        }
        p_ensure_used(t + 1);
        p_used[t] = 1;
        while (true) {
            p_ensure_used(p_minun + 1);
            if (!p_used[p_minun]) break;
            p_minun++;
        }
        p_prev = t;
        p_count++;
        if (t > p_max) p_max = t;
        if (p_max == p_count) {
            if (p_nrec == p_caprec) {
                p_caprec = p_caprec ? p_caprec * 2 : 256;
                p_recs = (uint64_t *)realloc(p_recs, p_caprec * sizeof(uint64_t));
            }
            p_recs[p_nrec++] = p_count;
        }
    }
    return (seq_t)p_recs[(size_t)n - 1];
}
