/*
 * A190101  Number of transpose partition pairs of order n whose number of odd
 *          parts differ by numbers of the form 4*k + 2.
 * https://oeis.org/A190101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 141, u64 n <= 439,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Euler transform (period 32) of the sequence
 *   c = [1,–1,1,4,1,–1,1,2, 1,–1,1,4,1,–1,1,3,
 *        1,–1,1,4,1,–1,1,2, 1,–1,1,4,1,–1,1,1]
 * Behaviour: a(0)=0, a(1)=0, a(n)=Q_{n‑2} for n≥2
 * where Q[0]=1 and
 *   Q[m] = (1/m) * Σ_{k=1..m} S(k) * Q[m‑k]
 * with S(k)=Σ_{d|k} d·c(d).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* the 32‑periodic sequence c(d) */
static const int8_t PERIOD[32] = {
    1, -1, 1, 4, 1, -1, 1, 2,
    1, -1, 1, 4, 1, -1, 1, 3,
    1, -1, 1, 4, 1, -1, 1, 2,
    1, -1, 1, 4, 1, -1, 1, 1
};

/* cache for S(k) = Σ_{d|k} d·c(d) */
static int64_t *S = NULL;
static idx_t S_cap = 0;

/* compute S(k) for all 1 ≤ k ≤ N, reallocating if necessary */
static void compute_S(idx_t N)
{
    if (N == 0) return;
    if (N + 1 > S_cap) {
        int64_t *tmp = (int64_t *)realloc(S, (N + 1) * sizeof(int64_t));
        if (!tmp) abort();
        S = tmp;
        S_cap = N + 1;
    }
    for (idx_t k = 1; k <= N; ++k) {
        int64_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int32_t c_d = PERIOD[(d - 1) & 31];
                sum += (int64_t)d * (int64_t)c_d;
                idx_t e = k / d;
                if (e != d) {
                    int32_t c_e = PERIOD[(e - 1) & 31];
                    sum += (int64_t)e * (int64_t)c_e;
                }
            }
        }
        S[k] = sum;
    }
}

/* cache for Q(m) */
static seq_t *Q = NULL;
static idx_t Q_cap = 0;

/* compute Q(m) for all 0 ≤ m ≤ M, reallocating if necessary */
static void compute_Q(idx_t M)
{
    if (M == (idx_t)-1) return;      /* no values needed */
    if (M + 1 > Q_cap) {
        seq_t *tmp = (seq_t *)realloc(Q, (M + 1) * sizeof(seq_t));
        if (!tmp) abort();
        Q = tmp;
        Q_cap = M + 1;
    }
    Q[0] = (seq_t)1;                    /* base case */
    compute_S(M);
    for (idx_t m = 1; m <= M; ++m) {
        if (sizeof(seq_t) <= 8) {        /* 64‑bit build: use 128‑bit to avoid overflow */
            __uint128_t acc = 0;
            for (idx_t k = 1; k <= m; ++k) {
                int64_t sk = S[k];
                seq_t q = Q[m - k];
                if (sk >= 0) {
                    acc += (__uint128_t)q * (unsigned long long)sk;
                } else {
                    acc -= (__uint128_t)q * (unsigned long long)(-sk);
                }
            }
            Q[m] = (seq_t)(acc / m);
        } else {                         /* arbitrary‑precision build: use seq_t arithmetic */
            seq_t acc = 0;
            for (idx_t k = 1; k <= m; ++k) {
                int64_t sk = S[k];
                seq_t q = Q[m - k];
                if (sk >= 0) {
                    acc += q * (seq_t)sk;
                } else {
                    acc -= q * (seq_t)(-sk);
                }
            }
            Q[m] = acc / (seq_t)m;
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    compute_Q(n - 2);
    return Q[n - 2];
}
