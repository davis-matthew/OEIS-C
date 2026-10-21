/*
 * A056345  Number of bracelets of length n using exactly five different
 *          colored beads.
 * https://oeis.org/A056345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (120/4) * (S₂(⌊(n+1)/2⌋,5)+S₂(⌈(n+1)/2⌉,5)) * +
 *            (120/(2n)) * Σ_{d|n} φ(d) * S₂(n/d,5)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 5, u64 n <= 22, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  Stirling numbers of the second kind  S₂(n,5)  ---------- */

static seq_t *S2 = NULL;        /* 1‑D array: row * 6 + k  (k = 0 … 5) */
static size_t S2_rows = 0;      /* highest n already computed */

/* allocate / grow S2 so that rows 0 … n are defined */
static void ensure_S2(size_t n)
{
    if (S2 == NULL) {
        S2 = (seq_t *)malloc(6 * sizeof(seq_t));
        for (int k = 0; k <= 5; ++k) S2[k] = 0;
        S2[0] = 1;      /* S₂(0,0)=1 ; all other entries stay 0 */
        S2_rows = 0;
    }
    if (n <= S2_rows) return;

    size_t new_rows = n + 1;          /* include new row n */
    S2 = (seq_t *)realloc(S2, new_rows * 6 * sizeof(seq_t));

    for (size_t i = S2_rows + 1; i <= n; ++i) {
        /* S₂(i,0) = 0 for i>0 – already zero because of realloc */
        for (int k = 1; k <= 5; ++k) {
            seq_t term = (seq_t)k * S2[(i - 1) * 6 + k];
            term += S2[(i - 1) * 6 + (k - 1)];
            S2[i * 6 + k] = term;
        }
    }
    S2_rows = n;
}

/* ----------  Euler totient φ(n)  ---------- */

static uint64_t *phi = NULL;
static size_t phi_size = 0;      /* maximum n already built for φ */

/* build φ values up to n using a simple sieve */
static void ensure_phi(size_t n)
{
    if (phi_size >= n) return;

    phi = (uint64_t *)realloc(phi, (n + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= n; ++i) phi[i] = i;
    if (n >= 2) {
        for (size_t i = 2; i <= n; ++i) {
            if (phi[i] == i) {          /* i is prime */
                for (size_t j = i; j <= n; j += i)
                    phi[j] -= phi[j] / i;
            }
        }
    }
    phi_size = n;
}

/* ----------  Main sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    size_t N = (size_t)n;
    ensure_S2(N);            /* need rows up to n */
    ensure_phi(N);           /* need φ up to n */

    /* ----------  first (reflection) term  ---------- */
    size_t a_part = (N + 1) / 2;      /* floor((n+1)/2) */
    size_t b_part = N / 2 + 1;        /* ceil((n+1)/2)  */

    seq_t s_a = S2[a_part * 6 + 5];
    seq_t s_b = S2[b_part * 6 + 5];
    seq_t term_ref = (seq_t)30 * (s_a + s_b);   /* 120/4 = 30 */

    /* ----------  second (rotation) term  ---------- */
    seq_t sum_rot = 0;
    for (size_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            size_t d1 = i;
            size_t d2 = N / i;

            size_t q1 = N / d1;   /* n/d1  */
            seq_t s_q1 = S2[q1 * 6 + 5];
            sum_rot += s_q1 * (seq_t)phi[d1];

            if (d1 != d2) {       /* distinct divisor pair */
                size_t q2 = N / d2;
                seq_t s_q2 = S2[q2 * 6 + 5];
                sum_rot += s_q2 * (seq_t)phi[d2];
            }
        }
    }
    /* 120/(2n) = 60/n */
    seq_t term_rot = (sum_rot * (seq_t)60) / (seq_t)N;

    return term_ref + term_rot;
}
