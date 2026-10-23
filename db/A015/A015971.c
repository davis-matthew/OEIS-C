/*
 * A015971  k is the first integer such that phi(k + n) | sigma(k).
 * https://oeis.org/A015971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015971
 * a(n) = the smallest k such that φ(k + n) divides σ(k)
 * where φ is Euler's totient function and σ is the sum of divisors.
 *
 * The function uses a simple SPF sieve to factor each required number
 * on the fly.  This is fast enough for the required range (n up to
 * ~10 000) and uses only the standard library.
 */

#include <stdint.h>
#include <stdlib.h>

/* static SPF table and its current maximum index */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure the SPF table contains data up to at least 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;

    /* free existing table if any */
    free(spf);

    /* allocate new table */
    spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf)
        exit(EXIT_FAILURE);

    /* initialise SPF values to the index itself */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i;

    /* classic SPF sieve */
    for (size_t i = 2; (size_t)i * i <= limit; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* Compute φ(m) using the SPF table. */
static seq_t phi_of(idx_t m)
{
    idx_t temp = m;
    seq_t result = (seq_t) m;          /* initial value of phi */

    while (temp > 1) {
        idx_t p = spf[temp];
        result -= result / (seq_t)p;  /* phi -= phi/p */
        while (temp % p == 0)
            temp /= p;
    }
    return result;
}

/* Compute σ(m) (sum of divisors) using the SPF table. */
static seq_t sigma_of(idx_t m)
{
    idx_t temp = m;
    seq_t result = 1;                  /* multiplicative identity */

    while (temp > 1) {
        idx_t p = spf[temp];
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* Compute p^(exp+1) as seq_t */
        seq_t p_pow = 1;
        for (int i = 0; i < exp + 1; ++i)
            p_pow *= (seq_t)p;

        /* term = (p^(exp+1) - 1) / (p - 1) */
        seq_t term = (p_pow - 1) / ((seq_t)p - 1);
        result *= term;
    }
    return result;
}

/* The required OEIS sequence function */
seq_t a(idx_t n)
{
    /* At least one entry is required for the loop */
    ensure_spf(1);

    for (idx_t k = 1;; ++k) {
        idx_t needed = k + n;
        if (needed > spf_limit)
            ensure_spf(needed);       /* grow sieve as far as needed */

        seq_t phi_val = phi_of(needed);
        seq_t sigma_val = sigma_of(k);

        if (sigma_val % phi_val == 0)
            return (seq_t)k;          /* k satisfies the condition */
    }
}
