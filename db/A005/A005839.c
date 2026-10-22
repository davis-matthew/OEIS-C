/*
 * A005839  Lexicographically earliest increasing nonnegative sequence that
 *          contains no 4-term arithmetic progression.
 * https://oeis.org/A005839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   greedy search: a(1) = 0, and a(n) is the smallest number >
 *            a(n-1) that does not complete a 4-term arithmetic progression
 *            with earlier terms. Each new term marks the values it would
 *            forbid
 * Range:     u8 n <= 74, u16 n <= 4077, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0, tcap = 0;
static unsigned char *flag = NULL; /* bit0: in sequence, bit1: forbidden */
static uint64_t fcap = 0;

static void ensure_flag(uint64_t need) {
    if (need <= fcap) return;
    uint64_t nc = fcap ? fcap : 1024;
    while (nc < need) nc *= 2;
    flag = (unsigned char *)realloc(flag, (size_t)nc);
    memset(flag + fcap, 0, (size_t)(nc - fcap));
    fcap = nc;
}

static void add_term(uint64_t x) {
    if (nterms >= tcap) {
        tcap = tcap ? tcap * 2 : 1024;
        terms = (uint64_t *)realloc(terms, (size_t)tcap * sizeof(uint64_t));
    }
    ensure_flag(2 * x + 2);
    /* x is the largest term: for each pair (q-d, q, x) forming a 3-AP, forbid x+d */
    for (idx_t i = 0; i < nterms; i++) {
        uint64_t q = terms[i];
        uint64_t d = x - q;
        if (q >= d && (flag[q - d] & 1))
            flag[x + d] |= 2;
    }
    terms[nterms++] = x;
    flag[x] |= 1;
}

seq_t a(idx_t n) {
    if (nterms == 0) add_term(0);
    while (nterms < n) {
        uint64_t c = terms[nterms - 1] + 1;
        while (flag[c] & 2) c++;
        add_term(c);
    }
    return (seq_t)terms[n - 1];
}
