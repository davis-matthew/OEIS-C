/*
 * A093958  a(n) = A091844(n) - 4.
 * https://oeis.org/A093958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091844(n) - 4, where A091844(1) = 4 and A091844(n+1) =
 *            max(k,4), with k the largest exponent such that the digit string
 *            of the sequence so far ends in y^k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9404 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static size_t *A093958_vals = NULL;
static size_t A093958_cnt = 0, A093958_vcap = 0;
static unsigned char *A093958_dig = NULL;
static size_t A093958_dlen = 0, A093958_dcap = 0;
static int *A093958_pi = NULL;
static size_t A093958_picap = 0;

static void A093958_push_digits(size_t v)
{
    char tmp[32];
    int pos = 0;
    if (v == 0) tmp[pos++] = 0;
    while (v) { tmp[pos++] = (char)(v % 10); v /= 10; }
    while (A093958_dlen + (size_t)pos > A093958_dcap) {
        A093958_dcap = A093958_dcap ? A093958_dcap * 2 : 256;
        A093958_dig = (unsigned char *)realloc(A093958_dig, A093958_dcap);
    }
    for (int i = pos - 1; i >= 0; --i)
        A093958_dig[A093958_dlen++] = (unsigned char)tmp[i];
}

static size_t A093958_best_exp(void)
{
    size_t len = A093958_dlen;
    if (len > A093958_picap) {
        A093958_picap = len * 2;
        A093958_pi = (int *)realloc(A093958_pi, A093958_picap * sizeof(int));
    }
    int *pi = A093958_pi;
    const unsigned char *d = A093958_dig;
    pi[0] = 0;
    size_t best = 1;
    size_t k = 0;
    for (size_t j = 1; j < len; ++j) {
        unsigned char cur = d[len - 1 - j];
        while (k > 0 && cur != d[len - 1 - k]) k = (size_t)pi[k - 1];
        if (cur == d[len - 1 - k]) ++k;
        pi[j] = (int)k;
        size_t L = j + 1;
        size_t p = L - k;
        if (L % p == 0) {
            size_t e = L / p;
            if (e > best) best = e;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    while (A093958_cnt < N) {
        size_t val;
        if (A093958_cnt == 0) val = 4;
        else {
            size_t e = A093958_best_exp();
            val = e > 4 ? e : 4;
        }
        if (A093958_cnt >= A093958_vcap) {
            A093958_vcap = A093958_vcap ? A093958_vcap * 2 : 256;
            A093958_vals = (size_t *)realloc(A093958_vals, A093958_vcap * sizeof(size_t));
        }
        A093958_vals[A093958_cnt++] = val;
        A093958_push_digits(val);
    }
    return (seq_t)(A093958_vals[N - 1] - 4);
}
