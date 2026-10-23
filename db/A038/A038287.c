/*
 * A038287  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*9^j.
 * https://oeis.org/A038287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 8^(i-j) * 9^j, * where i = floor(
 *            (sqrt(8n+1)-1)/2 ) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* multiplicative binomial with small‑overflow protection */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;

    seq_t val = 1;
    for (idx_t t = 1; t <= K; ++t) {
        seq_t num = (seq_t)(N - K + t);  /* numerator */
        seq_t den = (seq_t)t;            /* denominator */

        seq_t g1 = gcd_seq(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd_seq(val, den);
        val /= g2;
        den /= g2;      /* den should be 1 now */

        val = val * num;   /* val fits into seq_t when result fits */
    }
    return val;
}

/* fast binary exponentiation for seq_t */
static seq_t pow_seq_fixed(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL) result = result * base;
        base = base * base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find row i */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 <= n) {
        ++i;
    }

    /* column j */
    idx_t j = n - (i * (i + 1)) / 2;

    seq_t bin = binom(i, j);
    seq_t p8  = pow_seq_fixed((seq_t)8, i - j);
    seq_t p9  = pow_seq_fixed((seq_t)9, j);

    return bin * p8 * p9;
}
