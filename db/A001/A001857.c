/*
 * A001857  a(1)=2, a(2)=3; for n >= 3, a(n) is smallest number that is
 *          uniquely of the form a(j) + a(k) with 1 <= j < k < n.
 * https://oeis.org/A001857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   computed by a sieve over pair sums: a(1)=2, a(2)=3; a(n) =
 *            smallest s > a(n-1) whose count of representations a(j)+a(k),
 *            j<k<n, equals 1
 * Range:     u8 n <= 50, u16 n <= 6164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *ul_terms = NULL;
static idx_t ul_len = 0, ul_cap = 0;
static unsigned char *ul_cnt = NULL;
static size_t ul_csz = 0;

static void ul_ensure(size_t need) {
    if (need <= ul_csz) return;
    size_t ncap = ul_csz ? ul_csz : 64;
    while (ncap < need) ncap *= 2;
    ul_cnt = (unsigned char *)realloc(ul_cnt, ncap);
    memset(ul_cnt + ul_csz, 0, ncap - ul_csz);
    ul_csz = ncap;
}

static void ul_push(uint64_t t) {
    if (ul_len >= ul_cap) {
        ul_cap = ul_cap ? ul_cap * 2 : 64;
        ul_terms = (uint64_t *)realloc(ul_terms, (size_t)ul_cap * sizeof(uint64_t));
    }
    ul_ensure((size_t)(2 * t + 2));
    for (idx_t i = 0; i < ul_len; i++) {
        size_t s = (size_t)(ul_terms[i] + t);
        if (ul_cnt[s] < 2) ul_cnt[s]++;
    }
    ul_terms[ul_len++] = t;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (ul_len == 0) {
        ul_push(2);
        ul_push(3);
    }
    while (ul_len < n) {
        uint64_t s = ul_terms[ul_len - 1] + 1;
        while (s < ul_csz && ul_cnt[s] != 1) s++;
        ul_push(s);
    }
    return (seq_t)ul_terms[n - 1];
}
