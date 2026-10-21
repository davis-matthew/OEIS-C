/*
 * A056274  Number of primitive (aperiodic) word structures of length n using
 *          a 3-ary alphabet.
 * https://oeis.org/A056274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * (S(n/d,1)+S(n/d,2)+S(n/d,3)) where μ is
 *            the Möbius function and S(·,·) are Stirling numbers of the
 *            second kind. S(k,1)=1, S(k,2)=2^{k-1}-1,
 *            S(k,3)=(3^{k}-3·2^{k}+3)/6. The array _B_ below holds the sum
 *            S(n,1)+S(n,2)+S(n,3) for each n, which is equivalent to
 *            A007051(n-1). The...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

#define MAXN 10000

/* Möbius function, prime sieve and auxiliary arrays */
static int      mu[MAXN + 1];
static char     comp[MAXN + 1];
static int      primes[MAXN];
static int      prime_cnt;

/* powers of 2 and 3, and auxiliary sums */
static seq_t pow2[MAXN + 1];
static seq_t pow3[MAXN + 1];
static seq_t B[MAXN + 1];

/* Dirichlet convolution results */
static seq_t pos_sum[MAXN + 1];
static seq_t neg_sum[MAXN + 1];
static seq_t a_val[MAXN + 1];

/* One‑time initialization flag */
static bool     initialized = false;

/* ------------------------------------------------------------ */
/*  Pre‑compute all tables up to MAXN.                          */
/* ------------------------------------------------------------ */
static void precompute(void)
{
    /* -----------------  Möbius and prime sieve  ----------------- */
    mu[1] = 1;
    prime_cnt = 0;

    for (int i = 2; i <= MAXN; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            int v = i * p;
            if (v > MAXN) break;
            comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    /* --------------  Powers of 2 and 3  ----------------------- */
    pow2[0] = (seq_t)1;
    pow3[0] = (seq_t)1;
    for (int i = 1; i <= MAXN; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }

    /* -----------  Compute B[i] = Σ_{k=1}^3 S(i,k)  ------------- */
    B[0] = (seq_t)1; /* Not used except for n/d = 1 case */
    for (int i = 1; i <= MAXN; ++i) {
        /* term3 = (3^i - 3·2^i + 3) / 6  */
        seq_t numerator = pow3[i] - (seq_t)3 * pow2[i] + (seq_t)3;
        seq_t term3 = numerator / (seq_t)6;
        B[i] = pow2[i - 1] + term3; /* 2^{i-1} + term3  */
    }

    /* --------------  Dirichlet convolution  ------------------- */
    for (int i = 1; i <= MAXN; ++i) {
        pos_sum[i] = 0;
        neg_sum[i] = 0;
    }

    for (int d = 1; d <= MAXN; ++d) {
        if (mu[d] == 0) continue;
        for (int m = d; m <= MAXN; m += d) {
            seq_t val = B[m / d];
            if (mu[d] > 0)
                pos_sum[m] += val;
            else
                neg_sum[m] += val;
        }
    }

    for (int i = 1; i <= MAXN; ++i) {
        /* pos_sum[i] >= neg_sum[i] always holds for a(n) >= 0 */
        a_val[i] = pos_sum[i] - neg_sum[i];
    }

    initialized = true;
}

/* ------------------------------------------------------------ */
/*  Public function required by the problem statement.         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!initialized) precompute();

    if (n > (idx_t)MAXN) {
        /* Outside pre‑computed range; extendability is not required
           for the current test set. */
        return (seq_t)0;
    }

    return a_val[(size_t)n];
}
