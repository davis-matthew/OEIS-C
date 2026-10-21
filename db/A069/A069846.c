/*
 * A069846  Number of distinct primes obtained by inserting a 5 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069846 : Number of distinct primes obtained by inserting a 5
   at all possible places in the decimal representation of n
   (including prefixing).  The routine builds a sieve up to 600 000
   once and then counts the distinct primes among the (len+1)
   candidates. */
#include <stdbool.h>
#include <stddef.h>

#define MAX_PRIME 600000U

static unsigned char prime_tbl[MAX_PRIME + 1];
static bool sieve_built = false;

/* Build the sieve lazily the first time the sequence is queried. */
static void build_sieve(void)
{
    size_t i, j;
    for (i = 0; i <= MAX_PRIME; ++i) prime_tbl[i] = 1;
    prime_tbl[0] = 0;
    prime_tbl[1] = 0;
    for (i = 2; i * i <= MAX_PRIME; ++i)
    {
        if (prime_tbl[i])
        {
            for (j = i * i; j <= MAX_PRIME; j += i)
                prime_tbl[j] = 0;
        }
    }
    sieve_built = true;
}

/* Test primality of a number that may exceed MAX_PRIME by using
   trial division with primes from the sieve up to sqrt(n). */
static bool is_prime_large(unsigned int n)
{
    if (n < 2) return false;
    if (n <= MAX_PRIME) return prime_tbl[n];
    unsigned int p;
    for (p = 2; (unsigned long long)p * p <= n; ++p)
    {
        if (prime_tbl[p] && n % p == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!sieve_built) build_sieve();

    if (n == 0) return (seq_t)0;  /* undefined, return 0 for safety */

    /* Extract decimal digits of n (MSB first). */
    unsigned int digits[64];
    size_t len = 0;
    idx_t tmp = n;
    if (tmp == 0)   /* n==0 is only for safety */
    {
        digits[0] = 0;
        len = 1;
    }
    else
    {
        while (tmp > 0)
        {
            digits[len++] = (unsigned int)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to MSB first */
        for (size_t i = 0; i < len / 2; ++i)
        {
            unsigned int x = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = x;
        }
    }

    /* Store distinct primes found. Maximum candidates = len+1. */
    unsigned int seen[64];
    size_t seen_cnt = 0;

    for (size_t pos = 0; pos <= len; ++pos)
    {
        unsigned int cand = 0;
        size_t i;
        for (i = 0; i < len; ++i)
        {
            if (i == pos) cand = cand * 10 + 5;
            cand = cand * 10 + digits[i];
        }
        if (pos == len) cand = cand * 10 + 5;  /* insertion at the end */

        if (is_prime_large(cand))
        {
            /* avoid duplicates */
            bool dup = false;
            for (size_t k = 0; k < seen_cnt; ++k)
            {
                if (seen[k] == cand) { dup = true; break; }
            }
            if (!dup) seen[seen_cnt++] = cand;
        }
    }

    return (seq_t)seen_cnt;
}
