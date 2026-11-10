/*
 * A072184  Complement of A072676.
 * https://oeis.org/A072184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k not in A072676, i.e. k such that NOT (4k-1 and
 *            2k+1 both prime, or some 0<i<k has 2i+1, 2k+2i-1, 2k+2i+1 all
 *            prime); found by sieve and search
 * Range:     u8 n <= 52, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static unsigned char *A072184_pr = NULL;
static uint64_t A072184_lim = 0;
static uint64_t *A072184_c = NULL;
static size_t A072184_len = 0, A072184_cap = 0;
static uint64_t A072184_k = 0;

static void A072184_sieve(uint64_t need)
{
    if (need < A072184_lim) return;
    uint64_t nl = need + 1;
    if (nl < 2 * A072184_lim) nl = 2 * A072184_lim;
    if (nl < 1024) nl = 1024;
    free(A072184_pr);
    A072184_pr = (unsigned char *)malloc((size_t)nl);
    if (!A072184_pr) abort();
    for (uint64_t i = 0; i < nl; i++) A072184_pr[i] = 1;
    A072184_pr[0] = 0;
    A072184_pr[1] = 0;
    for (uint64_t p = 2; p * p < nl; p++)
        if (A072184_pr[p])
            for (uint64_t m = p * p; m < nl; m += p) A072184_pr[m] = 0;
    A072184_lim = nl;
}

static bool A072184_in676(uint64_t k)
{
    A072184_sieve(4 * k + 4);
    if (A072184_pr[4 * k - 1] && A072184_pr[2 * k + 1]) return true;
    for (uint64_t i = 1; i < k; i++) {
        uint64_t p = 2 * i + 1;
        if (!A072184_pr[p]) continue;
        if (A072184_pr[2 * k + p - 2] && A072184_pr[2 * k + p]) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A072184_len < (size_t)n) {
        ++A072184_k;
        if (!A072184_in676(A072184_k)) {
            if (A072184_len >= A072184_cap) {
                size_t nc = A072184_cap ? A072184_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A072184_c, nc * sizeof(uint64_t));
                if (!t) abort();
                A072184_c = t;
                A072184_cap = nc;
            }
            A072184_c[A072184_len++] = A072184_k;
        }
    }
    return (seq_t)A072184_c[(size_t)(n - 1)];
}
