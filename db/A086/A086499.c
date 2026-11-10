/*
 * A086499  Primes arising in A086498: a(n) = (2n)-th partial sum of A086498.
 * https://oeis.org/A086499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. A086498 takes primes in pairs: the first prime of each
 *            pair is the smallest prime not used yet; the second is the
 *            smallest unused prime that makes the running sum prime. a(n) is
 *            the running sum after n pairs
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *P = NULL;      /* primes in increasing order */
static unsigned char *U = NULL; /* used flags for those primes */
static size_t PC = 0;           /* number of primes */
static uint64_t LIM = 0;        /* sieve limit */
static uint64_t *S = NULL;      /* S[k] = a(k) */
static size_t SC = 0, SCAP = 0;
static size_t lo = 0;           /* index of the smallest unused prime */
static uint64_t run = 0;        /* running sum */

static void grow_primes(void) {
    uint64_t nl = LIM ? LIM * 2 : 1 << 16;
    unsigned char *c = (unsigned char *)calloc((size_t)nl + 1, 1);
    size_t cnt = 0;
    uint64_t i, j;
    uint64_t *np;
    unsigned char *nu;
    for (i = 2; i <= nl; i++) {
        if (!c[i]) {
            cnt++;
            if (i * i <= nl)
                for (j = i * i; j <= nl; j += i) c[j] = 1;
        }
    }
    np = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    nu = (unsigned char *)calloc(cnt, 1);
    cnt = 0;
    for (i = 2; i <= nl; i++) if (!c[i]) np[cnt++] = i;
    if (U) memcpy(nu, U, PC);
    free(c); free(P); free(U);
    P = np; U = nu; PC = cnt; LIM = nl;
}

seq_t a(idx_t n) {
    while (SC < n) {
        size_t j;
        uint64_t p;
        if (SC + 1 >= SCAP) {
            SCAP = SCAP ? SCAP * 2 : 1024;
            S = (uint64_t *)realloc(S, SCAP * sizeof(uint64_t));
        }
        while (lo >= PC || U[lo]) {
            if (lo >= PC) grow_primes();
            else lo++;
        }
        p = P[lo];
        U[lo] = 1;
        run += p;
        j = lo + 1;
        for (;;) {
            if (j >= PC) grow_primes();
            if (!U[j] && is_prime_u64(run + P[j])) break;
            j++;
        }
        U[j] = 1;
        run += P[j];
        S[++SC] = run;
    }
    return (seq_t)S[n];
}
