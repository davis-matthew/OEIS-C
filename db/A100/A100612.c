/*
 * A100612  a(n) = (0! + 1! + ... + (p-1)!) mod p, where p = prime(n).
 * https://oeis.org/A100612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{p-1} k! (mod p) where p = prime(n)
 * Range:     u8 n <= 61, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --- Prime storage --- */
#define PRIME_TABLE_CAP 12000          /* enough for n ≤ 10000 */
static uint64_t prime_tbl[PRIME_TABLE_CAP];
static size_t  prime_cnt   = 0;        /* number of primes generated so far */
static uint64_t next_candidate = 1;    /* next number to test */

/* --- Result cache --- */
static seq_t results[PRIME_TABLE_CAP];
static idx_t result_cnt = 0;           /* number of a(n) values cached */

/* Generate primes until we have at least n primes */
static void generate_primes_to(size_t n)
{
    if (prime_cnt >= n)
        return;                       /* already enough */

    for (;;)
    {
        ++next_candidate;             /* start testing from 2 */

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_tbl[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            if (prime_cnt < PRIME_TABLE_CAP)
                prime_tbl[prime_cnt++] = next_candidate;
        }

        if (prime_cnt >= n)
            break;
    }
}

/* --- Main sequence routine --- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* defined as 0 for n = 0 */

    /* Compute missing values up to n */
    for (idx_t i = result_cnt; i < n; ++i)
    {
        /* Ensure that prime(i+1) exists */
        generate_primes_to((size_t)(i + 1));

        uint64_t p = prime_tbl[(size_t)i];   /* prime(n) */

        /* Sum of factorials modulo p */
        uint64_t term = 1;           /* 0! = 1 */
        uint64_t sum  = 1;           /* add 0! */

        for (uint64_t k = 1; k < p; ++k)
        {
            term = (term * k) % p;
            sum += term;
            if (sum >= p)
                sum -= p;            /* keep sum < 2p, one subtraction is enough */
        }

        results[i] = (seq_t)sum;
    }
    result_cnt = n;

    return results[(size_t)n - 1];
}
