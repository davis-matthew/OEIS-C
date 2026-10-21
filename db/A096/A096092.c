/*
 * A096092  Numbers whose digits can be permuted to get a proper divisor.
 * https://oeis.org/A096092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose digits can be permuted to obtain a proper divisor
   a(1) = 10, a(2) = 20, …  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_counts.h"

static uint32_t *spf = NULL;          /* smallest prime factor table */
static size_t   spf_limit = 0;        /* size of spf table */

/* Build SPF table up to given limit */
static void build_spf(size_t limit)
{
    free(spf);
    spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= limit; ++i)
        if (spf[i] == (uint32_t)i)
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
    spf[0] = 0; spf[1] = 1;
    spf_limit = limit;
}

/* Ensure SPF up to at least n */
static void ensure_spf(size_t n)
{
    if (n > spf_limit)
        build_spf(n * 2 + 100);
}

/* Test whether m has a proper divisor that is a permutation of its digits
   (allowing leading zeros in the permutation) */
static bool qualifies(uint64_t m)
{
    /* digit multiset of m */
    unsigned char cntm[10];
    digit_counts((seq_t)m, cntm);
    unsigned char zeros_m = cntm[0];

    /* factor m using SPF */
    uint64_t p[16], e[16];
    int nf = 0;
    uint64_t temp = m;
    while (temp > 1 && nf < 16) {
        uint32_t f = spf[(size_t)temp];
        uint64_t power = f;
        int exp = 1;
        temp /= f;
        while (temp % f == 0) {
            temp /= f;
            power *= f;
            ++exp;
        }
        p[nf] = f;
        e[nf] = exp;
        ++nf;
    }

    /* enumerate all divisors */
    uint64_t divs[128];
    size_t dc = 1;
    divs[0] = 1;
    for (int i = 0; i < nf; ++i) {
        size_t cur = dc;
        for (size_t d = 0; d < cur; ++d) {
            uint64_t v = divs[d];
            for (int k = 1; k <= e[i]; ++k) {
                v *= p[i];
                divs[dc++] = v;
            }
        }
    }

    /* test each proper divisor */
    for (size_t i = 0; i < dc; ++i) {
        uint64_t d = divs[i];
        if (d == m || d == 0) continue;
        unsigned char cntd[10];
        digit_counts((seq_t)d, cntd);
        if (cntd[0] > zeros_m) continue;
        bool ok = true;
        for (int dig = 1; dig <= 9; ++dig)
            if (cntd[dig] != cntm[dig]) { ok = false; break; }
        if (ok) return true;
    }
    return false;
}

/* Cache to answer a(n) in any order */
static seq_t  *cache = NULL;
static idx_t   cache_len = 0;
static idx_t   cache_cap = 0;
static uint64_t last_checked = 9;   /* numbers <10 are not considered */

/* Append value to cache with dynamic growth */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* ensure cache covers up to n */
    while (cache_len < n) {
        /* find next qualifying number */
        uint64_t candidate = last_checked + 1;
        while (true) {
            ensure_spf(candidate);
            if (qualifies(candidate))
                break;
            ++candidate;
        }
        append_cache((seq_t)candidate);
        last_checked = candidate;
    }
    return cache[n - 1];
}
