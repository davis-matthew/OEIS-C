/*
 * A107580  Numbers n such that (n + prime(n)) and (n+1 + prime(n+1)) are
 *          divisible by 5.
 * https://oeis.org/A107580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th positive integer m such that * (m + prime(m))
 *            and (m+1 + prime(m+1)) are divisible by 5
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated on demand and stored in a static table. */

#include <stdint.h>   /* Provides uint32_t etc. */
#include <stddef.h>   /* Provides size_t */

#define MAX_SIEVE 4000000U   /* upper limit for prime sieve     */
#define MAX_PRIMES 300000U   /* maximum number of primes stored */
#define MAX_SEQ   200000U   /* maximum number of sequence terms */

static unsigned char isprime[MAX_SIEVE + 1];
static uint32_t     primes[MAX_PRIMES];
static size_t       primes_cnt = 0;

static seq_t seq_vals[MAX_SEQ];
static size_t seq_len = 0;

/* Perform a simple sieve of Eratosthenes up to MAX_SIEVE */
static void init_sieve(void)
{
    size_t i, j;
    for (i = 0; i <= MAX_SIEVE; ++i) isprime[i] = 1;
    isprime[0] = isprime[1] = 0;

    for (i = 2; i * i <= MAX_SIEVE; ++i)
        if (isprime[i])
            for (j = i * i; j <= MAX_SIEVE; j += i)
                isprime[j] = 0;

    primes_cnt = 0;
    for (i = 2; i <= MAX_SIEVE && primes_cnt < MAX_PRIMES; ++i)
        if (isprime[i])
            primes[primes_cnt++] = (uint32_t)i;
}

/* The main function required by the harness */
seq_t a(idx_t n)
{
    static int initialized = 0;           /* 0 = not yet initialized */
    static uint32_t candidate = 1;        /* next integer to test */

    /* Initialise prime data on first call */
    if (!initialized)
    {
        init_sieve();
        initialized = 1;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        /* Ensure we have enough primes for candidate and candidate+1 */
        /* (primes_cnt > candidate+1 is guaranteed by the chosen MAX_SIEVE) */

        uint32_t p_n   = primes[candidate   - 1];  /* prime(candidate)   */
        uint32_t p_np1 = primes[candidate];      /* prime(candidate+1) */

        if ( ((candidate + p_n)   % 5 == 0) &&
             ((candidate + 1 + p_np1) % 5 == 0) )
        {
            if (seq_len >= MAX_SEQ)
            {
                /* Should never happen for requested n up to 10000 */
                return 0;
            }
            seq_vals[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return seq_vals[n - 1];
}
