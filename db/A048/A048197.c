/*
 * A048197  Numbers k for which binomial(k, floor(k/2)) has more unitary than
 *          non-unitary divisors.
 * https://oeis.org/A048197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) is the n‑th integer k such that the central binomial
 * coefficient C(k,⌊k/2⌋) has more unitary than non‑unitary divisors.
 * This holds iff in the prime factorisation of C(k,⌊k/2⌋) no exponent
 * is ≥3 and at most one exponent equals 2.  */
/* Algorithm: generate k = 1,2,…; for each k compute exponents
 * of C(k,⌊k/2⌋) via Legendre's formula over all primes ≤ k;
 * accept k when the above condition is satisfied.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*   Configuration                                                      */
/* --------------------------------------------------------------------- */
#define MAX_PRIME_LIMIT 200000      /* should cover k for n ≤ 10 000 */

/* --------------------------------------------------------------------- */
/*   Prime sieve (static global)                                       */
/* --------------------------------------------------------------------- */
static int   primes[MAX_PRIME_LIMIT];   /* prime list (values)          */
static size_t prime_cnt = 0;            /* number of primes              */
static bool   primes_built = false;     /* flag                       */

/* Sieve of Eratosthenes, builds primes up to MAX_PRIME_LIMIT. */
static void build_primes(void)
{
    if (primes_built) return;

    static bool composite[MAX_PRIME_LIMIT + 1];
    for (int i = 2; i <= MAX_PRIME_LIMIT; ++i)
    {
        if (!composite[i])
        {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= MAX_PRIME_LIMIT)
                for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    composite[j] = true;
        }
    }
    primes_built = true;
}

/* --------------------------------------------------------------------- */
/*   Legendre exponent of prime p in n!                                 */
/* --------------------------------------------------------------------- */
static inline int legendre(int n, int p)
{
    int e = 0;
    while (n)
    {
        n /= p;
        e += n;
    }
    return e;
}

/* --------------------------------------------------------------------- */
/*   Sequence storage                                                   */
/* --------------------------------------------------------------------- */
static seq_t    *seq = NULL;  /* array of terms */
static size_t   seq_len = 0;   /* number of terms stored */
static size_t   seq_cap = 0;   /* allocated capacity */
static uint64_t cur_k   = 0;   /* last processed k  */

/* Ensure sequence array has room for at least one more term. */
static void ensure_seq_capacity(void)
{
    if (seq_len >= seq_cap)
    {
        size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*   Test k for the required property                                   */
/* --------------------------------------------------------------------- */
static bool k_is_valid(uint64_t k)
{
    int a = k >> 1;            /* floor(k/2)  */
    int b = (int)(k - a);      /* ceil(k/2)   */

    int count_two = 0;

    /* iterate over primes ≤ k */
    for (size_t idx = 0; idx < prime_cnt; ++idx)
    {
        int p = primes[idx];
        if (p > k) break;

        int e = legendre((int)k, p) - legendre(a, p) - legendre(b, p);
        if (e >= 3)                /* exponent ≥3 → fail */
            return false;
        if (e == 2)
            ++count_two;
        if (count_two > 1)          /* more than one 2 → fail */
            return false;
    }
    return true;   /* satisfies condition */
}

/* --------------------------------------------------------------------- */
/*   Generate next valid k                                              */
/* --------------------------------------------------------------------- */
static void generate_next(void)
{
    do
    {
        ++cur_k;
    } while (!k_is_valid(cur_k));

    /* store the term */
    ensure_seq_capacity();
    seq[seq_len++] = (seq_t)cur_k;
}

/* --------------------------------------------------------------------- */
/*   Public sequence function                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, return 0 */

    /* initialise on first call */
    if (!primes_built) build_primes();

    while (seq_len < n)
        generate_next();

    return seq[n - 1];   /* zero‑based indexing in seq array */
}
