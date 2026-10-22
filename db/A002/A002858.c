/*
 * A002858  Ulam numbers: a(1) = 1; a(2) = 2; for n>2, a(n) = least number >
 *          a(n-1) which is a unique sum of two distinct earlier terms.
 * https://oeis.org/A002858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 5021, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ulam numbers: a(1) = 1, a(2) = 2; a(n) = least number > a(n-1) that is a unique sum of two distinct earlier terms. Computed by a sieve that counts pairwise sums. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *ulam_terms = NULL;
static idx_t ulam_count = 0;
static idx_t ulam_cap = 0;
static unsigned char *ulam_freq = NULL;
static uint64_t ulam_fcap = 0;

static void ulam_ensure_freq(uint64_t idx) {
    if (idx >= ulam_fcap) {
        uint64_t nc = ulam_fcap ? ulam_fcap : 1024;
        while (nc <= idx) nc *= 2;
        ulam_freq = (unsigned char *)realloc(ulam_freq, (size_t)nc);
        memset(ulam_freq + ulam_fcap, 0, (size_t)(nc - ulam_fcap));
        ulam_fcap = nc;
    }
}

static void ulam_push(uint64_t v) {
    idx_t j;
    if (ulam_count >= ulam_cap) {
        idx_t nc = ulam_cap ? ulam_cap * 2 : 256;
        ulam_terms = (uint64_t *)realloc(ulam_terms, (size_t)nc * sizeof(uint64_t));
        ulam_cap = nc;
    }
    ulam_ensure_freq(2 * v + 1);
    for (j = 0; j < ulam_count; j++) {
        uint64_t s = ulam_terms[j] + v;
        if (ulam_freq[s] < 2) ulam_freq[s]++;
    }
    ulam_terms[ulam_count++] = v;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (ulam_count == 0) {
        ulam_push(1);
        ulam_push(2);
    }
    while (ulam_count < n) {
        uint64_t c = ulam_terms[ulam_count - 1] + 1;
        while (ulam_freq[c] != 1) c++;
        ulam_push(c);
    }
    return (seq_t)ulam_terms[n - 1];
}
