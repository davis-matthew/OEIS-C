/*
 * A141404  Irregular array: For any prime p that divides n, if the highest
 *          power of the prime p that divides n is p^b(n,p), then p^b(n,p) =
 *          Sum_{k=1..m} a(n,k), where m is the order of the prime-power
 *          p^b(n,p) among the prime-powers (each being the highest power of
 *          each prime q that divides n, where q divid...
 * https://oeis.org/A141404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = element at position n when the rows of the irregular
 *            array are listed sequentially (row 1 = (1), row 2 = (2), ...).
 *            Row n contains A001221(n) terms: for each prime p dividing n
 *            with highest power p^e, let p^e = sum_{k=1..m} a(n,k) where m is
 *            the rank of p^e among the prime‑powers of n when...
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* constants for the sieve */
#define SIEVE_LIMIT   200000          /* enough for n up to about 10000 */

/* static storage for smallest prime factor and primes list */
static uint32_t spf[SIEVE_LIMIT + 1];
static uint32_t primes[SIEVE_LIMIT];
static size_t  prime_cnt = 0;
static bool    sieve_done = false;

/* build linear sieve for spf and primes up to SIEVE_LIMIT */
static void build_sieve(void)
{
    for (idx_t i = 2; i <= (idx_t)SIEVE_LIMIT; ++i)
    {
        if (spf[i] == 0)           /* i is prime */
        {
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            uint32_t p = primes[j];
            if (p > spf[i]) break;
            idx_t v = i * (idx_t)p;
            if (v > (idx_t)SIEVE_LIMIT) break;
            spf[v] = p;
        }
    }
    sieve_done = true;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (!sieve_done) build_sieve();

    idx_t pos = 0;                      /* linear position so far */
    for (idx_t row = 1;; ++row)         /* iterate rows */
    {
        seq_t row_vals[10];             /* max distinct primes < 10 for our limits */
        size_t m;

        if (row == 1)                   /* special case */
        {
            m = 1;
            row_vals[0] = (seq_t)1;
        }
        else
        {
            /* factorise row using spf */
            idx_t tmp = row;
            seq_t powers[10];
            size_t mp = 0;
            while (tmp > 1)
            {
                uint32_t p = spf[tmp];
                int exp = 0;
                while (tmp % (idx_t)p == 0)
                {
                    tmp /= (idx_t)p;
                    ++exp;
                }
                seq_t pp = 1;
                for (int k = 0; k < exp; ++k) pp *= (seq_t)p;
                powers[mp++] = pp;
            }

            /* sort the prime powers ascending (simple bubble sort) */
            for (size_t i = 0; i < mp; ++i)
                for (size_t j = i + 1; j < mp; ++j)
                    if (powers[j] < powers[i])
                    {
                        seq_t t = powers[i];
                        powers[i] = powers[j];
                        powers[j] = t;
                    }

            /* compute row values as successive differences */
            for (size_t i = 0; i < mp; ++i)
                row_vals[i] = (i == 0) ? powers[0] : powers[i] - powers[i - 1];
            m = mp;
        }

        /* emit values until we hit the requested position */
        for (size_t i = 0; i < m; ++i)
        {
            ++pos;
            if (pos == n) return row_vals[i];
        }
    }
}
