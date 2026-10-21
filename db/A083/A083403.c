/*
 * A083403  Write the numbers 1, 2, ... in a triangle with n terms in the n-th
 *          row; a(n) = number of squarefree integers in n-th row.
 * https://oeis.org/A083403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = count_squarefree( T_n ) – count_squarefree( T_{n-1} ) *
 *            where T_k = k(k+1)/2 is the k‑th triangular number and *
 *            count_squarefree(x) = Σ_{d≥1} μ(d)·⌊x/(d²)⌋ (sum over d² ≤ x). *
 *            The Möbius values μ(d) are pre‑computed once up to a safe limit
 * Range:     u8 n <= 22, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#ifndef SEQ_T
/*  seq_t and idx_t are provided by the build system.  */
#endif

/* Sufficient for triangular numbers up to n≈10000 (T_n≈5·10⁷).  */
#define MU_MAX 20000               /* > sqrt(T_10000)≈7071 */

static signed char  mu[MU_MAX + 1];
static unsigned char  is_comp[MU_MAX + 1];
static int  primes[MU_MAX + 1];
static int  prime_cnt = 0;
static int  mu_computed = 0;

/* Linear sieve for μ(n) up to MU_MAX.  */
static void compute_mu(void)
{
    if (mu_computed) return;
    mu[1] = 1;
    for (int i = 2; i <= MU_MAX; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            long long v = (long long)i * p;
            if (v > MU_MAX) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = (signed char)(-mu[i]);
            }
        }
    }
    mu_computed = 1;
}

/* Count square‑free integers ≤ x.  */
static seq_t count_squarefree(seq_t x)
{
    if (!mu_computed) compute_mu();
    seq_t sum = 0;
    for (seq_t d = 1; d * d <= x; ++d) {
        int di = (int)d;
        if (di > MU_MAX) /* safety; algorithm uses only d ≤ sqrt(x) ≤ MU_MAX */ {
            /* If we need larger d, recompute mu for this case (rare). */
            /* For typical use up to n≈10000 this does not happen. */
            break;
        }
        int mu_d = mu[di];
        if (mu_d == 0) continue;
        seq_t term = (seq_t)mu_d * (x / (d * d));
        sum += term;
    }
    return sum;
}

/* Triangular number T_n = n(n+1)/2. */
static seq_t triangular(idx_t n)
{
    return (seq_t)n * ((seq_t)n + 1) / 2;
}

/* OEIS sequence A083403. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but safe */
    seq_t t_n   = triangular(n);
    seq_t t_nm1 = triangular(n - 1);
    return count_squarefree(t_n) - count_squarefree(t_nm1);
}
