/*
 * A049812  a(n)=number of Farey fractions of order n that are <=1/8; cf.
 *          A049805.
 * https://oeis.org/A049812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Farey fractions of order n that are <= 1/8 *
 *            (including 0/1). This is computed as * a(n) = 1 + Σ_{q=2..n}
 *            φ(q, ⌊q/8⌋) * where φ(q, m) is the number of integers 1 ≤ p ≤ m
 *            with gcd(p,q)=1. * The counting for each q is done by
 *            inclusion–exclusion on the distinct * prime factors of q...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, exit, EXIT_FAILURE */

static int  *spf     = NULL;   /* smallest prime factor for each integer */
static idx_t spf_len = 0;      /* current size of spf array (indices 0..spf_len-1) */

/* Initialise or grow the smallest–prime‑factor array up to n (inclusive). */
static void ensure_spf(idx_t n)
{
    if (n < spf_len)
        return;

    /* Allocate a fresh array of size n+1 and set each entry to itself. */
    size_t new_len = (size_t)n + 1;
    int *new_spf = (int *)malloc(new_len * sizeof(int));
    if (!new_spf)
        exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= n; ++i)
        new_spf[i] = (int)i;

    /* Sieve: for each prime i set spf[j] = i for its multiples. */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (new_spf[i] == (int)i) {          /* i is prime */
            for (idx_t j = i * i; j <= n; j += i)
                if (new_spf[j] == (int)j)
                    new_spf[j] = (int)i;
        }
    }

    free(spf);
    spf     = new_spf;
    spf_len = n + 1;
}

/* Count coprime numerators for a given denominator q. */
static seq_t coprime_count(idx_t q)
{
    idx_t m = q / 8;              /* floor(q/8) */
    if (m == 0)
        return (seq_t)0;          /* no positive numerator */

    /* Extract distinct prime factors of q. */
    int  primes[32];              /* 32 is ample for q <= 10^7 */
    size_t pk = 0;
    idx_t temp = q;
    while (temp > 1) {
        int p = spf[temp];
        if (pk == 0 || primes[pk - 1] != p)
            primes[pk++] = p;
        while (temp % p == 0)
            temp /= p;
    }

    /* Inclusion‑exclusion over subsets of the distinct primes. */
    seq_t count = (seq_t)m;       /* start with all 1..m */
    size_t subsets = ((size_t)1 << pk);
    for (size_t mask = 1; mask < subsets; ++mask) {
        idx_t prod = 1;
        size_t bits = 0;
        for (size_t b = 0; b < pk; ++b) {
            if (mask & ((size_t)1 << b)) {
                ++bits;
                prod *= primes[b];
                if (prod > m)
                    break;           /* term will be zero */
            }
        }
        if (prod > m)
            continue;
        seq_t term = (seq_t)(m / prod);
        if (bits & 1)
            count -= term;
        else
            count += term;
    }

    return count;
}

/*  Main sequence function required by the exercise.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_spf(n);

    seq_t total = (seq_t)1;   /* accounts for 0/1 */

    for (idx_t q = 2; q <= n; ++q)
        total += coprime_count(q);

    return total;
}
