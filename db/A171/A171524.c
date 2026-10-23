/*
 * A171524  a(n) = numbers m such that are not the product of k-th nonprime
 *          number and k for any k >= 1.
 * https://oeis.org/A171524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers m that are NOT of the form k * nonprime(k) for
 *            any k ≥ 1. Computation: generate all products k * nonprime(k) up
 *            to a sufficient bound, sort them, then scan natural numbers and
 *            skip those products until the nth remaining integer is found
 * Range:     u8 none, u16 n <= 9, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utilities required by this implementation are provided by the build. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Helper: compare two seq_t values for qsort. */
static int seq_cmp(const void *a, const void *b) {
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Helper: binary search to test for membership. */
static bool is_excluded(const seq_t *arr, size_t len, seq_t value) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        seq_t v = arr[mid];
        if (v == value) return true;
        if (v < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined, but guard */

    /* Static initialization on first call. */
    static bool ready = false;
    enum { KMAX = 20000 }; /* sufficient for n up to at least 10 000 */
    static seq_t nonprime[ KMAX ];
    static seq_t excluded[ KMAX ];
    static size_t excnt = 0;

    if (!ready) {
        /* Build list of first KMAX nonprime numbers (including 1). */
        nonprime[0] = 1; /* 1 is nonprime by definition */
        size_t cnt = 1;
        idx_t cand = 2;
        while (cnt < KMAX) {
            if (!is_prime(cand))
                nonprime[cnt++] = (seq_t)cand;
            cand++;
        }

        /* Compute excluded products k*nonprime(k). */
        for (size_t i = 0; i < cnt; ++i) {
            excluded[i] = ((seq_t)(i + 1)) * nonprime[i];
        }
        excnt = cnt;
        /* Sort excluded values to enable binary search. */
        qsort(excluded, excnt, sizeof(seq_t), seq_cmp);
        ready = true;
    }

    /* Scan integers, skipping excluded products. */
    seq_t val = 2;      /* a(1) is 2 */
    idx_t count = 0;
    for (;;) {
        if (!is_excluded(excluded, excnt, val)) {
            count++;
            if (count == n)
                return val;
        }
        val++;
    }
}
