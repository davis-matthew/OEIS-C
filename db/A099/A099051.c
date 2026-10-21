/*
 * A099051  p*2^p - 1 where p is prime.
 * https://oeis.org/A099051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n * 2^p_n - 1 where p_n is the n‑th prime (OEIS
 *            A099051)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* power of two in seq_t, exponent given as idx_t.  Binary exponentiation.*/
static seq_t pow2_seq(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp != 0)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Pre‑compute enough primes using the Sieve of Eratosthenes.  The limit is
   chosen so that the first 10 000 primes (the 10 000th is 104 729) are
   included.  The array sizes are compile‑time constants. */
#define SIEVE_LIMIT 200000
static int prime_table[SIEVE_LIMIT];
static size_t prime_count;

/* One‑time initialization. */
static void init_primes(void)
{
    static unsigned char sieve[SIEVE_LIMIT + 1] = {0};
    size_t cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!sieve[i])
        {
            prime_table[cnt++] = i;
            if ((long long)i * i <= SIEVE_LIMIT)
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve[j] = 1;
        }
    }
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset is 1, below it we return 0 */
    static bool init_done = false;
    if (!init_done)
    {
        init_primes();
        init_done = true;
    }

    if (n > prime_count) return (seq_t)0;      /* guard: should not happen for n≤10000 */

    idx_t p = (idx_t)prime_table[n - 1];       /* nth prime, 1‑based */
    seq_t term = ((seq_t)p) * pow2_seq(p) - (seq_t)1;
    return term;
}
