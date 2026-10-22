/*
 * A005487  Starts 0, 4 and contains no 3-term arithmetic progression.
 * https://oeis.org/A005487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   greedy search (Stanley sequence S(0,4)): a(0)=0, a(1)=4, and
 *            each next term is the smallest integer > a(n-1) that is not
 *            2*a(j) - a(i) for any i < j < n, so no 3-term AP forms
 * Range:     u8 n <= 37, u16 n <= 1010, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *terms = NULL;
static size_t nterms = 0, capterms = 0;
static unsigned char *forbid = NULL;
static size_t forbid_len = 0;

static void ensure_forbid(size_t need) {
    if (need < forbid_len) return;
    size_t nl = forbid_len ? forbid_len : 1024;
    while (nl <= need) nl *= 2;
    unsigned char *p = (unsigned char *)realloc(forbid, nl);
    if (!p) abort();
    memset(p + forbid_len, 0, nl - forbid_len);
    forbid = p;
    forbid_len = nl;
}

static void push_term(uint64_t c) {
    if (nterms == capterms) {
        size_t nc = capterms ? capterms * 2 : 256;
        uint64_t *p = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
        if (!p) abort();
        terms = p;
        capterms = nc;
    }
    ensure_forbid((size_t)(2 * c + 1));
    for (size_t i = 0; i < nterms; i++)
        forbid[2 * c - terms[i]] = 1;
    terms[nterms++] = c;
}

seq_t a(idx_t n) {
    if (nterms == 0) {
        push_term(0);
        push_term(4);
    }
    while ((idx_t)nterms <= n) {
        uint64_t c = terms[nterms - 1] + 1;
        ensure_forbid((size_t)c);
        while (forbid[c]) {
            c++;
            ensure_forbid((size_t)c);
        }
        push_term(c);
    }
    uint64_t v = terms[n];
    seq_t r = 0;
    seq_t base = 1;
    while (v) {
        if (v & 1) r += base;
        base = base + base;
        v >>= 1;
    }
    return r;
}
