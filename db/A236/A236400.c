/*
 * A236400  Primes p=prime(k) such that min{r_p, p-r_p} <= 2, where r_p =
 *          A100612(k).
 * https://oeis.org/A236400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over primes p = prime(k) in increasing order; for each p,
 *            compute r_p = A100612(k) = (0! + 1! + ... + (p-1)!) mod p
 *            directly (16 primes in parallel, with exact double-precision
 *            balanced modular reduction), and keep p when min(r_p, p - r_p)
 *            <= 2
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define A236400_LIMIT 5000000u
#define A236400_CAP 64
#define A236400_LANES 16

typedef double A236400_v4d __attribute__((vector_size(32)));

static unsigned char A236400_comp[A236400_LIMIT + 1];
static bool A236400_sieved = false;
static uint64_t A236400_terms[A236400_CAP];
static int A236400_cnt = 0;
static uint32_t A236400_next = 3;   /* next odd candidate */

static void A236400_sieve(void)
{
    A236400_comp[0] = A236400_comp[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= A236400_LIMIT; i++)
        if (!A236400_comp[i])
            for (uint32_t j = i * i; j <= A236400_LIMIT; j += i)
                A236400_comp[j] = 1;
    A236400_sieved = true;
}

static void A236400_batch(void)
{
    const double MAG = 6755399441055744.0; /* 1.5 * 2^52: round-to-nearest trick */
    uint32_t P[A236400_LANES];
    int m = 0;
    while (m < A236400_LANES && A236400_next <= A236400_LIMIT) {
        if (!A236400_comp[A236400_next]) P[m++] = A236400_next;
        A236400_next += 2;
    }
    if (m == 0) return;
    for (int l = m; l < A236400_LANES; l++) P[l] = P[m - 1];

    A236400_v4d T[4], S[4], PV[4], IV[4], MV;
    for (int i = 0; i < 4; i++) MV[i] = MAG;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++) {
            double pd = (double)P[4 * j + i];
            PV[j][i] = pd;
            IV[j][i] = 1.0 / pd;
            T[j][i] = 1.0;   /* 0! */
            S[j][i] = 1.0;   /* sum so far includes 0! */
        }

    uint32_t pmin = P[0];
    for (uint32_t k = 1; k < pmin; k++) {
        double kd = (double)k;
        A236400_v4d KV;
        KV[0] = kd; KV[1] = kd; KV[2] = kd; KV[3] = kd;
        for (int j = 0; j < 4; j++) {
            A236400_v4d x = T[j] * KV;
            A236400_v4d q = (x * IV[j] + MV) - MV;
            T[j] = x - q * PV[j];
            S[j] += T[j];
        }
    }

    for (int l = 0; l < m; l++) {
        uint32_t p = P[l];
        double pd = (double)p, ip = 1.0 / pd;
        double t = T[l >> 2][l & 3];
        double s = S[l >> 2][l & 3];
        for (uint32_t k = pmin; k < p; k++) {
            double x = t * (double)k;
            double q = (x * ip + MAG) - MAG;
            t = x - q * pd;
            s += t;
        }
        long long r = (long long)s % (long long)p;
        if (r < 0) r += (long long)p;
        if ((r <= 2 || (long long)p - r <= 2) && A236400_cnt < A236400_CAP)
            A236400_terms[A236400_cnt++] = p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!A236400_sieved) {
        A236400_sieve();
        A236400_terms[A236400_cnt++] = 2; /* p = 2: 0!+1! = 2 = 0 mod 2 */
    }
    while ((idx_t)A236400_cnt < n && A236400_next <= A236400_LIMIT)
        A236400_batch();
    if ((idx_t)A236400_cnt >= n) return (seq_t)A236400_terms[n - 1];
    return (seq_t)0;
}
