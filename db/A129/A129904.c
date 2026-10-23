/*
 * A129904  Find the first two terms in A003215, say A003215(i) and
 *          A003215(j), that are divisible by a number in A016921 not 1, say
 *          by k = A016921(m). Then i + j + 1 = k and k is added to the
 *          sequence.
 * https://oeis.org/A129904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A129904: numbers congruent to 1 (mod 6) whose
 * prime factors are all congruent to 1 (mod 6).
 *
 * The sequence is generated lazily and memoised so that any
 * order of calls to a(idx_t n) works correctly.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Prime generation – sieve of Eratosthenes up to PRIME_LIMIT            */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 2000000   /* Sufficient for the first 10 000 terms */

static uint32_t *prime_list = NULL;  /* array of primes up to PRIME_LIMIT */
static size_t   prime_cnt   = 0;     /* number of primes stored */

/* Initialise the prime list once */
static void init_primes(void)
{
    if (prime_list != NULL) return;    /* already initialised */

    bool *is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (is_prime == NULL) exit(EXIT_FAILURE);  /* allocation failed */

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (prime_list == NULL)
        exit(EXIT_FAILURE);

    size_t pos = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[pos++] = (uint32_t)i;

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/*  Check whether every prime divisor of x is ≡ 1 (mod 6)                  */
/* --------------------------------------------------------------------- */
static bool good_prime_factors(seq_t x)
{
    seq_t temp = x;

    /* Iterate over all primes up to sqrt(temp) */
    for (size_t idx = 0; idx < prime_cnt; ++idx)
    {
        uint32_t p = prime_list[idx];
        uint64_t square = (uint64_t)p * (uint64_t)p;
        if ((seq_t)square > temp)
            break;

        if (temp % (seq_t)p == 0)
        {
            if (p % 6 != 1)
                return false;

            /* Strip all copies of this factor */
            while (temp % (seq_t)p == 0)
                temp /= (seq_t)p;
        }
    }

    /* Remaining part is either 1 or a prime > sqrt(original) */
    if (temp > (seq_t)1)
    {
        if (temp % (seq_t)6 != (seq_t)1)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Sequence storage                                                    */
/* --------------------------------------------------------------------- */
static seq_t  *seq       = NULL;      /* array of found terms          */
static size_t  seq_len   = 0;         /* number of terms stored        */
static size_t  seq_cap   = 0;         /* current allocation capacity   */
static idx_t   last_test = 1;         /* smallest candidate already tested */

/* --------------------------------------------------------------------- */
/*  Main accessor                                                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for non‑positive indices */

    /* Initialise once on first call */
    if (prime_list == NULL)
        init_primes();

    if (seq == NULL)
    {
        seq_cap = 8;                  /* initial capacity */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have enough */
    while (seq_len < (size_t)n)
    {
        ++last_test;                 /* next integer to examine */

        if (last_test % 6ULL != 1ULL)
            continue;                /* only consider 6k+1 numbers */

        seq_t cand = (seq_t)last_test;

        if (!good_prime_factors(cand))
            continue;                /* not all prime factors ≡ 1 (mod 6) */

        /* Grow array if necessary */
        if (seq_len == seq_cap)
        {
            seq_cap = (seq_cap << 1) | 1;   /* double and make odd */
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (seq == NULL) exit(EXIT_FAILURE);
        }

        seq[seq_len++] = cand;
    }

    return seq[(size_t)n - 1];
}
