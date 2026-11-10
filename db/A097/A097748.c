/*
 * A097748  Smallest m such that A097747(m) = n.
 * https://oeis.org/A097748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that A097747(m) = n, where A097747(0) =
 *            1, A097747(m) = A097747(m mod A097747(m-1)) + floor(m /
 *            A097747(m-1)); found by running the recurrence incrementally and
 *            recording first occurrences
 * Range:     u8 n <= 21, u16 n <= 289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9078 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A097748_LIM ((uint64_t)1 << 22)

static uint32_t *A097748_val = NULL;   /* first values of A097747 */
static uint64_t A097748_vcap = 0;
static uint64_t *A097748_first = NULL; /* first[v] = smallest m + 1, 0 if unseen */
static uint64_t A097748_fcap = 0;
static uint64_t A097748_m = 0;         /* next index to compute */
static uint64_t A097748_prev = 0;      /* A097747(m-1) */

static void A097748_record(uint64_t m, uint64_t v)
{
    if (m < A097748_LIM) {
        if (m >= A097748_vcap) {
            uint64_t nc = A097748_vcap ? A097748_vcap * 2 : 1024;
            uint32_t *t = (uint32_t *)realloc(A097748_val, nc * sizeof(uint32_t));
            if (!t) abort();
            A097748_val = t;
            A097748_vcap = nc;
        }
        A097748_val[m] = (uint32_t)v;
    }
    if (v >= A097748_fcap) {
        uint64_t nc = A097748_fcap ? A097748_fcap : 1024;
        while (nc <= v) nc *= 2;
        uint64_t *t = (uint64_t *)realloc(A097748_first, nc * sizeof(uint64_t));
        if (!t) abort();
        for (uint64_t i = A097748_fcap; i < nc; ++i) t[i] = 0;
        A097748_first = t;
        A097748_fcap = nc;
    }
    if (A097748_first[v] == 0) A097748_first[v] = m + 1;
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    if (A097748_m == 0) {
        A097748_record(0, 1);
        A097748_prev = 1;
        A097748_m = 1;
    }
    while (!(target < A097748_fcap && A097748_first[target] != 0)) {
        uint64_t m = A097748_m;
        uint64_t p = A097748_prev;
        uint64_t r = m % p;
        uint64_t v = (uint64_t)A097748_val[r] + m / p;
        A097748_record(m, v);
        A097748_prev = v;
        A097748_m = m + 1;
    }
    return (seq_t)(A097748_first[target] - 1);
}
