/*
 * A100478  a(1)=a(2)=a(3)=a(4)=a(5)=1; for n>5, a(n) = pi(Sum_{j=1..5}
 *          a(n-j)) where pi = A000720.
 * https://oeis.org/A100478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = pi( a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5) ), with
 *            a(1)=a(2)=a(3)=a(4)=a(5)=1. pi(k) is the prime‑counting function
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external integer types: seq_t (output) and idx_t (index), provided by build.  */

/* --------------------------------------------------------------------
   Prime counting helper
   -------------------------------------------------------------------- */
static unsigned char *is_prime = NULL;   /* 0 = composite, 1 = prime, index 0..limit */
static unsigned int  *pi_tbl   = NULL;   /* pi_tbl[i] = pi(i) */
static idx_t         max_prime = 0;     /* highest index computed so far */

/* Re‑compute sieve up to at least "limit".  The arrays are reallocated
   to size (limit+1).  For the small limits required here (<=~330) this
   is trivial cost.  */
static void init_prime_sieve(idx_t limit)
{
    if (limit <= max_prime)
        return;

    /* Allocate / resize arrays */
    unsigned char *new_prime = (unsigned char*)realloc(is_prime, (limit + 1) * sizeof(unsigned char));
    unsigned int  *new_pi    = (unsigned int *)realloc(pi_tbl,    (limit + 1) * sizeof(unsigned int));
    if (!new_prime || !new_pi)
        abort();

    is_prime = new_prime;
    pi_tbl   = new_pi;

    /* initial assumption: all numbers >=2 are prime */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* cumulative pi table */
    pi_tbl[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi_tbl[i] = pi_tbl[i - 1] + is_prime[i];

    max_prime = limit;
}

/* prime counting query */
static inline seq_t pi_func(idx_t x)
{
    if (x > max_prime)
        init_prime_sieve(x);
    return (seq_t)pi_tbl[x];
}

/* --------------------------------------------------------------------
   Sequence a(n)
   -------------------------------------------------------------------- */
static seq_t *dp      = NULL;   /* dp[1..max_computed], dp[0] unused */
static idx_t  dp_len  = 0;      /* current allocated size (dp_len-1 usable) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n<1, defensive guard */

    /* Initialisation of base terms */
    if (dp_len == 0) {
        dp_len = 6;                     /* indices 0..5 usable */
        dp = (seq_t*)calloc(dp_len, sizeof(seq_t));
        if (!dp) abort();
        for (idx_t i = 1; i <= 5; ++i)
            dp[i] = (seq_t)1;
        /* ensure sieve covers the smallest possible sum (5) */
        init_prime_sieve(5);
    }

    /* Extend dp until it covers index n */
    while (n >= dp_len) {
        /* allocate one more slot */
        seq_t *new_dp = (seq_t*)realloc(dp, (dp_len + 1) * sizeof(seq_t));
        if (!new_dp) abort();
        dp = new_dp;

        /* compute next term (at index dp_len) */
        seq_t sum = dp[dp_len - 1] + dp[dp_len - 2] + dp[dp_len - 3]
                  + dp[dp_len - 4] + dp[dp_len - 5];

        /* sum fits in idx_t for our limits, but cast anyway */
        dp[dp_len] = pi_func((idx_t)sum);

        ++dp_len;
    }

    return dp[n];
}
