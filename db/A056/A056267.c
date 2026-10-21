/*
 * A056267  Number of primitive (aperiodic) words of length n which contain
 *          exactly two different symbols.
 * https://oeis.org/A056267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * A000918(n/d) with A000918(k) = 2^k - 2
 *            (for k >= 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t    capacity   = 1;   /* size of allocated tables (>=1)          */
static int     *mu_tbl    = NULL; /* Möbius values for 1..capacity-1        */
static seq_t   *pow2_tbl  = NULL; /* powers of two for 0..capacity-1        */
static int     *primes    = NULL; /* primes list for sieve (size capacity)  */

/* Extend all helper tables up to at least n (inclusive) */
static void extend_tables(idx_t n)
{
    if (n < capacity - 1) return;            /* already large enough */

    /* Reallocate tables to hold 0..n inclusive */
    size_t sz = (size_t)n + 1;
    mu_tbl   = (int*)            realloc(mu_tbl,   sz * sizeof(int));
    pow2_tbl = (seq_t*)          realloc(pow2_tbl, sz * sizeof(seq_t));
    primes   = (int*)            realloc(primes,   sz * sizeof(int));

    bool *is_comp = (bool*)calloc(sz, sizeof(bool));

    /* Möbius sieve (linear) */
    size_t pcnt = 0;
    mu_tbl[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[pcnt] = (int)i;
            ++pcnt;
            mu_tbl[i] = -1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = (idx_t)primes[j];
            idx_t ip = p * i;
            if (ip > n) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                mu_tbl[ip] = 0;
                break;
            } else {
                mu_tbl[ip] = -mu_tbl[i];
            }
        }
    }
    free(is_comp);

    /* Pre‑compute powers of two */
    pow2_tbl[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2_tbl[i] = pow2_tbl[i-1] * (seq_t)2;

    capacity = n + 1;   /* store the new capacity */
}

/* Public function requested in the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for accidental zero calls */

    extend_tables(n);              /* ensure tables cover 'n' */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t k = n / d;
        seq_t f = pow2_tbl[k] - (seq_t)2;   /* A000918(k) = 2^k - 2 */

        int mu_val = mu_tbl[d];
        if (mu_val == 1)   sum += f;
        else if (mu_val == -1) sum -= f;    /* mu_val == 0: do nothing */

        idx_t d2 = n / d;
        if (d2 == d) continue;             /* avoid double counting */

        k = n / d2;
        f = pow2_tbl[k] - (seq_t)2;
        mu_val = mu_tbl[d2];
        if (mu_val == 1)   sum += f;
        else if (mu_val == -1) sum -= f;
    }

    return sum;   /* non‑negative for all n >= 1 */
}
