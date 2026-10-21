/*
 * A099866  Denominator of sum of all elements M(i,j,k) = i*j/k, (i,j,k =
 *          1..n). a(n) =
 *          Denominator[Sum[Sum[Sum[i*j/k,{i,1,n}],{j,1,n}],{k,1,n}]].
 * https://oeis.org/A099866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099866
   a(n) = denominator of [ ((n(n+1)/2)^2) * H_n ],
   where H_n = ∑_{k=1..n} 1/k  (harmonic number). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Euclidean algorithm – works for all seq_t types that support + - * / % . */
static seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* not part of the sequence */
    size_t N = (size_t)n;

    /* ----------  Sieve of Eratosthenes  ---------- */
    bool *is_comp = (bool*)malloc((N + 1) * sizeof(bool));
    if (!is_comp) return 0;
    for (size_t i = 2; i <= N; ++i) is_comp[i] = false;

    idx_t *primes = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!primes) { free(is_comp); return 0; }
    size_t primes_cnt = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (idx_t)i;
            if (i * i <= N) {
                for (size_t j = i * i; j <= N; j += i) is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* ----------  Compute L = lcm(1..n)  ---------- */
    seq_t L = 1;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        seq_t p = (seq_t)primes[pi];
        /* exponent e of p in L */
        size_t e = 0;
        seq_t power = p;
        while (power <= (seq_t)n) {
            ++e;
            power *= p;
        }
        /* compute p^e */
        seq_t powp = 1;
        for (size_t i = 0; i < e; ++i) powp *= p;
        L *= powp;
    }
    free(primes);

    /* ----------  Compute S = Σ_{k=1..n} L/k  ---------- */
    seq_t S = 0;
    for (size_t k = 1; k <= N; ++k) {
        S += L / (seq_t)k;
    }

    /* ----------  Denominator of H_n  ---------- */
    seq_t g1   = gcd_seq_t(L, S);
    seq_t denomH = L / g1;

    /* ----------  Reduce with (n(n+1)/2)^2  ---------- */
    seq_t T   = (seq_t)n * (seq_t)(n + 1) / 2;
    seq_t T2  = T * T;
    seq_t g2  = gcd_seq_t(denomH, T2);
    seq_t denomFinal = denomH / g2;

    return denomFinal;
}
