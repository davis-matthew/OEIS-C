/*
 * A156225  Triangle read by rows:e(n,k)=Sum[(-1)^j Binomial[n + 1, j](k + 1 -
 *          j)^n, {j, 0, k + 1}]; t(n,m)=(e[n + 1, m]*PartitionsQ[n] + e[n +
 *          1, n - m]*(PartitionsQ[ n - m] + PartitionsQ[m])) - 2.
 * https://oeis.org/A156225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = t(r,c) where r = floor((sqrt(8n+1)-1)/2) and c = n -
 *            r(r+1)/2. t(r,c) = e(r+1,c) * P(r) + e(r+1,r-c) * (P(r-c)+P(c))
 *            - 2 with e(n,k) = sum_{j=0}^{k+1} (-1)^j * binom(n+1,j) *
 *            (k+1-j)^n and P(x) = number of partitions of x into distinct
 *            parts
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: multiplicative binomial coefficient. */
static seq_t binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i);
        res = res / i;   /* exact division */
    }
    return res;
}

/* Helper: integer power using repeated multiplication. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result = result * base;
    return result;
}

/* Compute e(n,k) as defined. */
static seq_t compute_e(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= k + 1; ++j) {
        seq_t b = binom(n + 1, j);
        idx_t pow_base = k + 1 - j;
        seq_t pow_val = pow_seq((seq_t)pow_base, n);
        seq_t term = b * pow_val;
        if (j & 1u)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

/* Pre‑compute partitions into distinct parts up to a maximum size. */
#define PART_MAX 2000          /* sufficient for all rows needed for n <= 10⁴ */
static seq_t partitions[PART_MAX + 1];

/* Compute partitions[0..n] inclusively. */
static void compute_partitions(idx_t n)
{
    if (n > PART_MAX) return;  /* safety; not expected for n <= 10⁴ */
    partitions[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        partitions[i] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = n; j >= i; --j) {
            partitions[j] = partitions[j] + partitions[j - i];
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine row r and column c for the n‑th element. */
    idx_t r = 0;
    idx_t offset = 0;
    while (n >= offset + r + 1) {
        offset += r + 1;
        ++r;
    }
    idx_t c = n - offset;

    /* Compute partition numbers up to r. */
    compute_partitions(r);

    /* Compute the triangle entry t(r,c). */
    seq_t e1 = compute_e(r + 1, c);
    seq_t e2 = compute_e(r + 1, r - c);
    seq_t val = e1 * partitions[r] + e2 * (partitions[r - c] + partitions[c]) - ((seq_t)2);
    return val;
}
