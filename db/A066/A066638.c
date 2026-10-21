/*
 * A066638  Smallest power of a squarefree number that is a multiple of n.
 * https://oeis.org/A066638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = rad(n)^mpe(n), where rad(n) is the product of distinct
 *            primes dividing n and mpe(n) = max exponent of any prime in the
 *            prime factorization of n (equivalently a(n) = rad(n)^max_k), for
 *            n ≥ 1. The function computes this value for any n ≥ 1. It uses a
 *            simple trial–division prime list up to 1,...
 * Range:     u8 n <= 39, u16 n <= 159, u32 n <= 1343, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static prime list */
static int *prime_list = NULL;          /* array of primes */
static size_t prime_cnt = 0;            /* number of primes */

/* allocate primes up to MAXP using a simple sieve */
static void init_primes(void) {
    if (prime_cnt > 0) return;          /* already initialized */
    const int MAXP = 1000000;           /* 1e6 */
    unsigned char *is_composite = (unsigned char*)calloc(MAXP + 1, 1);
    if (!is_composite) return;          /* allocation failure, unlikely */
    size_t cap = 78498;                 /* number of primes <= 1e6 */
    prime_list = (int*)malloc(cap * sizeof(int));
    if (!prime_list) { free(is_composite); return; }
    size_t idx = 0;
    for (int i = 2; i <= MAXP; ++i) {
        if (!is_composite[i]) {
            prime_list[idx++] = i;
            if ((int64_t)i * i <= MAXP)
                for (int j = i * i; j <= MAXP; j += i)
                    is_composite[j] = 1;
        }
    }
    prime_cnt = idx;
    free(is_composite);
}

/* compute rad(n) and max exponent, return rad and set maxexp via pointer */
static seq_t compute_rad_and_maxexp(idx_t n, idx_t *maxexp_out) {
    seq_t rad = 1;
    idx_t tmp = n;
    idx_t maxexp = 0;
    init_primes();                       /* ensure primes are ready */

    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= tmp; ++i) {
        int p = prime_list[i];
        if (tmp % p == 0) {
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            rad *= (seq_t)p;             /* multiply once per distinct prime */
            if (cnt > maxexp) maxexp = cnt;
        }
    }
    if (tmp > 1) {                       /* remaining prime factor */
        rad *= (seq_t)tmp;
        if (1 > maxexp) maxexp = 1;
    }
    *maxexp_out = maxexp;
    return rad;
}

/* integer power by repeated multiplication */
static seq_t power_seq_t(seq_t base, idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined behaviour for n<1, but safe */
    idx_t maxexp;
    seq_t rad = compute_rad_and_maxexp(n, &maxexp);
    return power_seq_t(rad, maxexp);
}
