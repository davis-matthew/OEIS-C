/*
 * A075697  a(n) = sum-of-digits(A075696(n)).
 * https://oeis.org/A075697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digit sum of A075696(n); A075696 found by scanning k with
 *            a growing prime sieve and testing whether digitsum(k) is a
 *            substring of prime(k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9403 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *g_terms = NULL;
static size_t g_nterms = 0, g_cterms = 0;
static uint64_t g_k = 1;            /* next k to test */
static uint32_t *g_pr = NULL;       /* primes */
static size_t g_np = 0;
static uint64_t g_limit = 0;

static void A075697_regrow(void)
{
    g_limit = g_limit ? g_limit * 2 : (1u << 21);
    unsigned char *s = (unsigned char *)calloc((size_t)g_limit + 1, 1);
    if (!s) abort();
    free(g_pr);
    g_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)g_limit / 8 + 1000));
    if (!g_pr) abort();
    g_np = 0;
    for (uint64_t i = 2; i <= g_limit; i++) {
        if (!s[i]) {
            g_pr[g_np++] = (uint32_t)i;
            if (i * i <= g_limit)
                for (uint64_t j = i * i; j <= g_limit; j += i) s[j] = 1;
        }
    }
    free(s);
}

static unsigned A075697_dsum(uint64_t k)
{
    unsigned s = 0;
    while (k) { s += (unsigned)(k % 10); k /= 10; }
    return s;
}

seq_t a(idx_t n)
{
    while (g_nterms < (size_t)n) {
        if (g_k > g_np) { A075697_regrow(); continue; }
        uint64_t k = g_k++;
        unsigned ds = A075697_dsum(k);
        char pb[16], db[16];
        snprintf(pb, sizeof pb, "%u", g_pr[k - 1]);
        snprintf(db, sizeof db, "%u", ds);
        if (strstr(pb, db)) {
            if (g_nterms == g_cterms) {
                g_cterms = g_cterms ? g_cterms * 2 : 1024;
                g_terms = (uint32_t *)realloc(g_terms, g_cterms * sizeof(uint32_t));
                if (!g_terms) abort();
            }
            g_terms[g_nterms++] = (uint32_t)k;
        }
    }
    return (seq_t)A075697_dsum(g_terms[(size_t)n - 1]);
}
