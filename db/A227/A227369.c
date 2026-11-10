/*
 * A227369  Positions where A227183 obtains new distinct values for the first
 *          time.
 * https://oeis.org/A227369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (k=0,1,2,...) such that A227183(k) differs from
 *            A227183(j) for all j<k; computed by an incremental scan using
 *            the run-length definition of A227183
 * Range:     u8 n <= 20, u16 n <= 72, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Validated: standard, pass, 69 terms through n = 127 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A227369_val(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t sum = 0, cum = 0;
    int run = 0;
    while (n > 0) {
        uint64_t bit = n & 1ULL;
        uint64_t len = 0;
        while (n > 0 && (n & 1ULL) == bit) { len++; n >>= 1; }
        uint64_t t = (run > 0) ? len - 1 : len;
        cum += t;
        sum += cum;
        run++;
    }
    return sum;
}

static unsigned char A227369_seen[16384];
static uint64_t *A227369_terms = 0;
static size_t A227369_cnt = 0, A227369_cap = 0;
static uint64_t A227369_next = 0;

seq_t a(idx_t n)
{
    while (A227369_cnt <= (size_t)n) {
        uint64_t k = A227369_next++;
        uint64_t v = A227369_val(k);
        if (v < 16384 && !A227369_seen[v]) {
            A227369_seen[v] = 1;
            if (A227369_cnt == A227369_cap) {
                size_t nc = A227369_cap ? A227369_cap * 2 : 256;
                A227369_terms = (uint64_t *)realloc(A227369_terms, nc * sizeof(uint64_t));
                A227369_cap = nc;
            }
            A227369_terms[A227369_cnt++] = k;
        }
    }
    return (seq_t)A227369_terms[(size_t)n];
}
