/*
 * A143326  Table T(n,k) by antidiagonals. T(n,k) is the number of primitive
 *          (=aperiodic) k-ary words with length less than or equal to n (n,k
 *          >= 1).
 * https://oeis.org/A143326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = ∑_{d=1}^{n} k^d * M(floor(n/d)), where M(x) =
 *            ∑_{i=1}^{x} μ(i) is the Mertens function. The OEIS sequence a(n)
 *            is obtained by traversing the (n,k) table along antidiagonals:
 *            (1,1), (1,2), (2,1), (1,3), (2,2), (3,1), … a(n) = T(n,k) for
 *            the pair (n,k) corresponding to the n‑th position on...
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 163, u64 n <= 423,
 *            u128 n <= 1112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t  current_max = 0;        /* current pre‑computed limit   */
static int   *mu          = NULL;     /* Möbius values 1 … current_max*/
static int   *M           = NULL;     /* Mertens prefix sums 0 … current_max */

/* Pre‑compute μ(i) and M(i) up to “required” using the linear sieve. */
static void
ensure_mu(idx_t required)
{
    if (required <= current_max)
        return;

    size_t sz = required + 1;
    int   *mu_new = (int *)calloc(sz, sizeof(int));
    int   *M_new  = (int *)calloc(sz, sizeof(int));
    bool  *comp   = (bool *)calloc(sz, sizeof(bool));
    int   *prime  = (int *)calloc(sz, sizeof(int));
    if (!mu_new || !M_new || !comp || !prime)
        exit(EXIT_FAILURE);

    mu_new[1] = 1;
    M_new[0] = 0;
    M_new[1] = 1;
    int primes_cnt = 0;

    for (idx_t i = 2; i <= required; ++i) {
        if (!comp[i]) {
            prime[primes_cnt++] = (int)i;
            mu_new[i] = -1;
        }
        for (int j = 0; j < primes_cnt; ++j) {
            idx_t p = (idx_t)prime[j];
            idx_t v = p * i;
            if (v > required) break;
            comp[v] = true;
            if (i % p == 0) {
                mu_new[v] = 0;
                break;
            } else {
                mu_new[v] = -mu_new[i];
            }
        }
    }

    for (idx_t i = 1; i <= required; ++i)
        M_new[i] = M_new[i - 1] + mu_new[i];

    free(mu);
    free(M);
    free(comp);
    free(prime);
    mu = mu_new;
    M  = M_new;
    current_max = required;
}

/* Multiply a seq_t value by a small positive integer without overflow. */
static seq_t
mul_small_seq(seq_t v, unsigned int m)
{
    seq_t res = (seq_t)0;
    for (unsigned int i = 0; i < m; ++i)
        res += v;
    return res;
}

/* Evaluate T(n,k) using the Mertens form. */
static seq_t
T_value(idx_t n, idx_t k)
{
    if (n == 0 || k == 0)
        return (seq_t)0;

    ensure_mu(n);

    seq_t sum = (seq_t)0;
    seq_t cur = (seq_t)1;             /* current k^d, start with k^0 */

    for (idx_t d = 1; d <= n; ++d) {
        cur *= (seq_t)k;              /* k^d */
        int m_val = M[n / d];        /* M(floor(n/d)) */
        if (m_val == 0)
            continue;
        if (m_val > 0)
            sum += mul_small_seq(cur, (unsigned int)m_val);
        else
            sum -= mul_small_seq(cur, (unsigned int)(-m_val));
    }
    return sum;
}

/* Convert linear index (1‑based) to the (n,k) pair on the antidiagonals. */
static void
index_to_pair(idx_t idx, idx_t *n_out, idx_t *k_out)
{
    if (idx == 0) {
        *n_out = 0;
        *k_out = 0;
        return;
    }

    /* Find smallest S with (S-1)*S/2 >= idx. */
    idx_t S = 2;
    while ((S - 1) * S / 2 < idx)
        ++S;

    idx_t prev_terms = (S - 2) * (S - 1) / 2; /* terms before this diagonal */
    idx_t offset = idx - prev_terms - 1;       /* 0‑based offset on diagonal */

    *n_out = offset + 1;
    *k_out = S - *n_out;
}

/* Required public function. */
seq_t a(idx_t idx)
{
    if (idx == 0)
        return (seq_t)0;   /* guard against 0, though OEIS offset is 1 */
    idx_t n, k;
    index_to_pair(idx, &n, &k);
    return T_value(n, k);
}
