/*
 * A126214  a(1)=1. a(n) = number of earlier terms, a(k) (for 1<=k<=n-1),
 *          where every integer coprime to a(k) and <= a(k) is also coprime to
 *          n.
 * https://oeis.org/A126214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A126214
 *
 * a(1) = 1
 * a(n) = number of earlier terms a(k) (1 ≤ k < n) such that
 *        every integer i with 1 ≤ i ≤ a(k) and gcd(i,a(k)) = 1
 *        is also coprime to n.
 *
 * The condition is equivalent to:
 *   For each prime divisor p of n, if p ≤ a(k) then p divides a(k).
 *
 * This function computes the sequence by iterating n through the
 * requested index, keeping a static cache of previously computed terms
 * and a static list of small primes for factorisation.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Prime sieve – initialise once and store primes up to MAX_PRIME_LIMIT  */
/* --------------------------------------------------------------------- */
#define MAX_PRIME_LIMIT 200000          /* sufficient for n up to ~10⁵     */
static uint64_t primes[MAX_PRIME_LIMIT];
static size_t   prime_count = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;

    /* Allocate and initialise to true */
    bool *is_prime = (bool *)malloc(MAX_PRIME_LIMIT + 1);
    if (!is_prime)
        return;                     /* allocation failure – function will fail */

    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
    {
        if (!is_prime[i])
            continue;
        primes[prime_count++] = i;
        if (i <= MAX_PRIME_LIMIT / i)
        {
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    free(is_prime);
    primes_ready = true;
}

/* --------------------------------------------------------------------- */
/* Cache for previously computed terms                                   */
/* --------------------------------------------------------------------- */
static seq_t  *seq_buf = NULL;
static idx_t  seq_cap = 0;   /* allocated capacity                           */
static idx_t  seq_len = 0;   /* number of values actually stored              */

static void ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = (need > 8 ? need + need / 2 : 8);
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Small prime factor list for a single number (distinct primes only)   */
/* --------------------------------------------------------------------- */
static uint64_t pf[64];
static size_t   pf_cnt;

/* ------------------------------------------------------------ */
/* Factorise n (uint64_t) – store distinct prime divisors in pf. */
/* ------------------------------------------------------------ */
static void factor_64(uint64_t n)
{
    pf_cnt = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i)
    {
        uint64_t p = primes[i];
        if (temp % p == 0)
        {
            pf[pf_cnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        pf[pf_cnt++] = temp;            /* remaining prime factor > sqrt(n) */
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined index – return 0 */

    init_primes();                     /* ensure primes table ready */

    /* Initialise cache on first call */
    if (seq_cap == 0)
    {
        ensure_seq_cap(n);
        seq_buf[0] = (seq_t)1;         /* a(1) = 1 */
        seq_len = 1;
    }

    /* Compute new values up to requested n */
    if (n > seq_len)
    {
        ensure_seq_cap(n);

        for (idx_t cur = seq_len + 1; cur <= n; ++cur)
        {
            factor_64((uint64_t)cur);   /* primes of current n */

            idx_t cnt = 0;
            for (idx_t k = 0; k < cur - 1; ++k)
            {
                seq_t m = seq_buf[k];
                bool ok = true;
                for (size_t i = 0; i < pf_cnt; ++i)
                {
                    uint64_t p = pf[i];
                    if ((seq_t)p <= m && (m % (seq_t)p) != 0)
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                    ++cnt;
            }
            seq_buf[cur - 1] = (seq_t)cnt;
        }
        seq_len = n;
    }
    return seq_buf[n - 1];
}
