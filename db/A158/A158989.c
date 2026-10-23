/*
 * A158989  Numbers n such that product of double factorials of the digits of
 *          n equals sigma(n).
 * https://oeis.org/A158989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate the sequence by testing integers in increasing order
   until the desired number of terms has been found. The sequence consists
   of numbers n such that the product of double factorials of the decimal
   digits of n equals sigma(n). This program is fully static, uses no
   dynamic allocation for the sequence itself, and works for any seq_t /
   idx_t supplied by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Double–factorial values for decimal digits */
static const seq_t dd[10] =
{
    1,   /* 0!! */
    1,   /* 1!! */
    2,   /* 2!! */
    3,   /* 3!! */
    8,   /* 4!! = 4·2 */
    15,  /* 5!! = 5·3·1 */
    48,  /* 6!! = 6·4·2 */
    105, /* 7!! = 7·5·3·1 */
    384, /* 8!! = 8·6·4·2 */
    945  /* 9!! = 9·7·5·3·1 */
};

/* Prime list for sigma computation */
static uint64_t *prime_list = NULL; /* array of primes */
static size_t     prime_cnt  = 0;   /* number of primes in prime_list */
static size_t     prime_cap  = 0;   /* allocated capacity of prime_list */
static size_t     prime_limit = 0;  /* maximal prime computed (incl.) */

/* Ensure prime_list contains all primes up to at least 'limit'. */
static void build_primes_up_to(size_t limit)
{
    if (limit <= prime_limit)
        return; /* already built */

    /* Allocate a small boolean array for the sieve. */
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp)
        exit(1); /* out of memory */

    /* Ensure prime_list has enough capacity. */
    size_t new_cap = (prime_cap == 0) ? 1024 : prime_cap;
    prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!prime_list)
    {
        free(is_comp);
        exit(1);
    }
    prime_cap = new_cap;
    prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            /* Store prime. */
            if (prime_cnt == prime_cap)
            {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
                if (!prime_list)
                {
                    free(is_comp);
                    exit(1);
                }
            }
            prime_list[prime_cnt++] = i;

            /* Mark composites. */
            if (i * i <= limit)
            {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    prime_limit = limit;
}

/* Sum of divisors sigma(n) for a 64‑bit integer n. */
static seq_t sigma_uint64(uint64_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 1;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i)
    {
        uint64_t p = prime_list[i];
        if (temp % p == 0)
        {
            seq_t term = 1;
            uint64_t power = 1;
            while (temp % p == 0)
            {
                temp /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }

    if (temp > 1)                /* residual prime factor */
        result *= (seq_t)(1 + temp);

    return result;
}

/* Product of double factorials of the decimal digits of n. */
static seq_t digit_dblfact_product(idx_t n)
{
    seq_t prod = 1;
    uint64_t x = n;
    while (x > 0)
    {
        unsigned d = x % 10;
        prod *= dd[d];
        x /= 10;
    }
    return prod;
}

/* Static storage for sequence terms.  The size is chosen to be comfortably
   larger than any term the test harness will request (e.g., n <= 10000). */
#define MAX_TERMS 100000
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;      /* how many terms have been found so far */
static idx_t  last_checked = 0;   /* largest integer already examined */

/* Ensure that 'terms' contains at least 'needed' terms. */
static void ensure_terms(idx_t needed)
{
    if (terms_len >= (size_t)needed)
        return;

    /* Pre‑compute a generous list of primes for sigma calculations. */
    if (prime_limit == 0)
        build_primes_up_to(2000000);   /* 2 000 000 is ample for the range needed */

    while (terms_len < (size_t)needed)
    {
        ++last_checked;

        seq_t prod = digit_dblfact_product(last_checked);
        seq_t sig  = sigma_uint64(last_checked);

        if (prod == sig)
        {
            if (terms_len == MAX_TERMS)
            {
                /* Should never happen for the intended use. */
                return;
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined below the offset */
    ensure_terms(n);
    return terms[n - 1];
}
