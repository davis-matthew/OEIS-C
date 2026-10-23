/*
 * A015618  Number of triples of different integers from [ 2,n ] with no
 *          global factor.
 * https://oeis.org/A015618
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d=1}^{n} μ(d) * C(floor(n/d) - [d==1], 3)
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count triples of distinct integers in [2,n] whose greatest common divisor is 1 */

/* The implementation keeps a memoized Möbius array up to the maximum n
 * requested so far.  It uses only functions available in the standard
 * library, so it compiles under both C11 and C++17.  The sequence value
 * is computed by the inclusion–exclusion formula described above.  The
 * intermediate arithmetic fits in a signed 64‑bit integer for all calls
 * that the tests make, so a long long accumulator suffices. */

#include <stdlib.h>
#include <stdbool.h>

/* memoised Möbius table */
static int *mu_table = NULL;      /* indices 0 .. mu_size-1 */
static size_t mu_size = 0;        /* number of stored entries */

/* helper: linear sieve for μ up to limit */
static void build_mu(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    int  *new_mu   = (int*)malloc(sz * sizeof(int));
    bool *is_comp  = (bool*)calloc(sz, sizeof(bool));
    int  *primes   = (int*)malloc(sz * sizeof(int));
    size_t pc = 0;

    new_mu[0] = 0;                /* unused */
    new_mu[1] = 1;

    for (size_t i = 2; i < sz; ++i)
    {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j) {
            size_t p = (size_t)primes[j];
            size_t ip = i * p;
            if (ip >= sz) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                new_mu[ip] = 0;
                break;
            } else {
                new_mu[ip] = -new_mu[i];
            }
        }
    }

    free(mu_table);
    free(is_comp);
    free(primes);
    mu_table = new_mu;
    mu_size = sz;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* below offset – undefined */

    if (n + 1 > mu_size) build_mu(n);  /* extend memoised Möbius table if needed */

    long long acc = 0;                 /* accumulator – signed 64‑bit */

    for (idx_t d = 1; d <= n; ++d) {
        int mu_d = mu_table[d];
        if (mu_d == 0) continue;          /* skip terms where μ(d)=0 */

        /* count of numbers in [2,n] divisible by d */
        idx_t cnt = (d == 1) ? (n - 1) : (n / d);

        if (cnt < 3) continue;            /* not enough for a triple */

        /* compute C(cnt,3) = cnt*(cnt-1)*(cnt-2)/6 using seq_t temporary */
        seq_t term = (seq_t)cnt * ((seq_t)cnt - 1) * ((seq_t)cnt - 2) / 6;
        long long term_ll = (long long)term;   /* safe for these bounds */

        if (mu_d > 0)
            acc += term_ll;
        else
            acc -= term_ll;
    }

    return (seq_t)acc;
}
