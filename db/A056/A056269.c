/*
 * A056269  Number of primitive (aperiodic) words of length n which contain
 *          exactly four different symbols.
 * https://oeis.org/A056269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * (4^(n/d) – 4·3^(n/d) + 6·2^(n/d) – 4)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   μ   is the Möbius function. */

/* Standard headers for C11 and C++17 */
#include <stdint.h>
#include <stdlib.h>

/* Static tables that grow on demand */
static idx_t   current_limit = 0;   /* largest n for which tables are valid */
static int   *mu = NULL;            /* Möbius values (−1,0,1) */
static seq_t *pow2 = NULL;          /* 2^i  */
static seq_t *pow3 = NULL;          /* 3^i  */
static seq_t *pow4 = NULL;          /* 4^i  */

/*-----------------   auxiliary: integer power (generic)  -----------------*/
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = (seq_t)1;
    while (exp) {
        if (exp & 1U) r = r * base;
        exp >>= 1U;
        if (exp) base = base * base;
    }
    return r;
}

/*-----------------   build tables up to at least n  ---------------------*/
static void grow_tables(idx_t n)
{
    if (n <= current_limit) return;           /* already sufficient  */

    /* Allocate new arrays with space [0..n] */
    int   *new_mu  = (int*)  malloc((n + 1) * sizeof(int ));
    seq_t *new_p2  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *new_p3  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *new_p4  = (seq_t*)malloc((n + 1) * sizeof(seq_t));

    /* Linear sieve for Möbius function up to n */
    {
        uint64_t *primes   = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
        int       pcnt     = 0;
        int8_t   *is_comp  = (int8_t*)calloc(n + 1, 1);

        new_mu[0] = 0;
        new_mu[1] = 1;
        for (idx_t i = 2; i <= n; ++i) {
            if (!is_comp[i]) {
                primes[pcnt++] = i;
                new_mu[i] = -1;
            }
            for (int j = 0; j < pcnt; ++j) {
                idx_t p = primes[j];
                idx_t v = i * p;
                if (v > n) break;
                is_comp[v] = 1;
                if (i % p == 0) {
                    new_mu[v] = 0;
                    break;
                } else {
                    new_mu[v] = -new_mu[i];
                }
            }
        }
        free(primes);
        free(is_comp);
    }

    /* Pre‑compute powers of 2,3,4 up to n (or 32 for 64‑bit seq_t)   */
    {
        idx_t max_pow = n;
        if (sizeof(seq_t) <= 8) {                /* 64‑bit or less */
            /* 2^32 < 2^64, beyond that overflow is expected */
            if (max_pow > 32) max_pow = 32;
        }
        new_p2[0] = (seq_t)1;
        new_p3[0] = (seq_t)1;
        new_p4[0] = (seq_t)1;
        for (idx_t i = 1; i <= max_pow; ++i) {
            new_p2[i] = new_p2[i-1] * (seq_t)2;
            new_p3[i] = new_p3[i-1] * (seq_t)3;
            new_p4[i] = new_p4[i-1] * (seq_t)4;
        }
    }

    /* Release old tables */
    free(mu);
    free(pow2);
    free(pow3);
    free(pow4);

    /* Adopt new tables */
    mu   = new_mu;
    pow2 = new_p2;
    pow3 = new_p3;
    pow4 = new_p4;
    current_limit = n;
}

/*--------------------------  main function  ----------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* sequence is defined only for n>=1 */

    /* Ensure tables cover n and all divisor arguments */
    grow_tables(n);

    seq_t result = (seq_t)0;

    /* Enumerate divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d) continue;

        idx_t q1 = n / d;           /* first divisor pair        */
        idx_t q2 = d;               /* second divisor pair (may equal q1) */

        /* Process divisor d */
        {
            idx_t k = q1;           /* exponent for f(k) */
            seq_t term = (seq_t)0;

            /* retrieve cached powers if possible, else compute directly */
            seq_t p2 = (k <= current_limit) ? pow2[k] : pow_seq((seq_t)2, k);
            seq_t p3 = (k <= current_limit) ? pow3[k] : pow_seq((seq_t)3, k);
            seq_t p4 = (k <= current_limit) ? pow4[k] : pow_seq((seq_t)4, k);

            term = p4;
            term = term - (seq_t)4 * p3;
            term = term + (seq_t)6 * p2;
            term = term - (seq_t)4;

            if (mu[d] == 1)   result += term;
            else if (mu[d] == -1) result -= term;
        }

        /* If the two divisors are distinct, process the paired divisor n/d */
        if (q1 != q2) {
            idx_t k = q2;           /* exponent for f(k) is d */
            seq_t term = (seq_t)0;
            seq_t p2 = (k <= current_limit) ? pow2[k] : pow_seq((seq_t)2, k);
            seq_t p3 = (k <= current_limit) ? pow3[k] : pow_seq((seq_t)3, k);
            seq_t p4 = (k <= current_limit) ? pow4[k] : pow_seq((seq_t)4, k);

            term = p4;
            term = term - (seq_t)4 * p3;
            term = term + (seq_t)6 * p2;
            term = term - (seq_t)4;

            if (mu[q1] == 1)   result += term;
            else if (mu[q1] == -1) result -= term;
        }
    }

    return result;
}
