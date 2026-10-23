/*
 * A016179  Inverse of 2170th cyclotomic polynomial.
 * https://oeis.org/A016179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse coefficients of the 2170‑th cyclotomic polynomial.
   Compute Φ₍₂₁₇₀₎(x) by the product formula
     Φₙ(x) = ∏_{d|n} (xᵈ – 1)^{μ(n/d)}.
   Then a(0) = 1 and for k ≥ 1
     a(k) = –∑_{i=1}^{deg(Φ)} φ[i] · a(k‑i).
   The sequence is periodic with period 2170. */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD          2170
#define MAX_NUM_DEG     2700          /* sufficient for intermediate
                                        polynomial degrees (2664) */
#define MAX_D           2170          /* maximal divisor of 2170 */

static seq_t a_seq[PERIOD];
static bool initialized = false;

/* Small Möbius function for square‑free arguments. */
static int mobius_small(idx_t m)
{
    int cnt = 0;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;      /* square factor */
            ++cnt;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) ++cnt;                      /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* Polynomial multiplication a(x) * b(x) → res(x). */
static void poly_mul(const seq_t *a, size_t degA,
                     const seq_t *b, size_t degB,
                     seq_t *res)
{
    size_t i, j;
    for (i = 0; i <= degA + degB; ++i) res[i] = 0;
    for (i = 0; i <= degA; ++i)
        for (j = 0; j <= degB; ++j)
            res[i + j] += a[i] * b[j];
}

/* Exact polynomial division num(x) / den(x) → quot(x).
   Assumes deg_num ≥ deg_den. */
static void poly_div_exact(const seq_t *num, size_t deg_num,
                           const seq_t *den, size_t deg_den,
                           seq_t *quot)
{
    size_t qdeg = (deg_num >= deg_den) ? deg_num - deg_den : 0;
    for (size_t i = 0; i <= qdeg; ++i) quot[i] = 0;

    seq_t rem[MAX_NUM_DEG + 1];
    for (size_t i = 0; i <= deg_num; ++i) rem[i] = num[i];

    for (int k = (int)deg_num; k >= (int)deg_den; --k) {
        seq_t coeff = rem[k];
        if (coeff == 0) continue;
        quot[k - (int)deg_den] = coeff;
        for (size_t j = 0; j <= deg_den; ++j)
            rem[k - (int)deg_den + j] -= coeff * den[j];
    }
}

/* Precompute one period of the sequence. */
static void init_sequence(void)
{
    /* 1. Build Φ₍₂₁₇₀₎(x) using the product formula. */
    const idx_t primes[4] = {2, 5, 7, 31};

    seq_t num[MAX_NUM_DEG + 1], den[MAX_NUM_DEG + 1];
    size_t deg_num = 0, deg_den = 0;
    num[0] = 1;      /* numerator starts as 1 */
    den[0] = 1;      /* denominator starts as 1 */

    seq_t factor[MAX_D + 1];
    seq_t tmp[MAX_NUM_DEG + 1];

    /* Iterate over all divisors of 2170. */
    for (int mask = 0; mask < 16; ++mask) {
        idx_t d = 1;
        for (int bit = 0; bit < 4; ++bit)
            if (mask & (1 << bit)) d *= primes[bit];

        idx_t m = PERIOD / d;
        int mu = mobius_small(m);

        /* Polynomial (xᵈ – 1). */
        for (size_t i = 0; i <= d; ++i) factor[i] = 0;
        factor[0] = -1;
        factor[d] = 1;

        if (mu == 1) {           /* numerator factor */
            poly_mul(num, deg_num, factor, d, tmp);
            deg_num += d;
            for (size_t i = 0; i <= deg_num; ++i) num[i] = tmp[i];
        } else if (mu == -1) {   /* denominator factor */
            poly_mul(den, deg_den, factor, d, tmp);
            deg_den += d;
            for (size_t i = 0; i <= deg_den; ++i) den[i] = tmp[i];
        }
        /* mu == 0: skip */
    }

    /* 2. Divide to obtain Φ₍₂₁₇₀₎(x). */
    seq_t phi[MAX_NUM_DEG + 1];
    size_t phi_deg = 0;
    poly_div_exact(num, deg_num, den, deg_den, phi);
    phi_deg = deg_num - deg_den;   /* degree of Φ (720) */

    /* 3. Compute the coefficients a(n) from the inverse series. */
    a_seq[0] = 1;
    for (size_t k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        size_t max_i = (k < phi_deg) ? k : phi_deg;
        for (size_t i = 1; i <= max_i; ++i)
            sum += phi[i] * a_seq[k - i];
        a_seq[k] = -sum;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (!initialized) { init_sequence(); initialized = true; }
    return a_seq[n % PERIOD];
}
