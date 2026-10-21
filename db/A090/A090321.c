/*
 * A090321  T(n,k) = prime(n+1) - prime(n-k+1), 1<=k<=n, triangular array read
 *          by rows.
 * https://oeis.org/A090321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(r+1) – prime(r-k+1), * where r is the row such that
 *            r(r+1)/2 >= n and k = n - r(r-1)/2
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The sequence is defined from
 *   T(n,k) = prime(n+1) - prime(n-k+1)   (1 ≤ k ≤ n)
 *   and a(n) is the sequence of T(n,k) read row‑by‑row.
 */

#include <stdint.h>
#include <stdlib.h>

/*----- Basic prime generation ------------------------------------------------*/

static seq_t *g_primes = NULL;    /* stored primes: 2,3,5,... */
static idx_t g_prime_count = 0;   /* number of primes stored */
static idx_t g_prime_cap   = 0;   /* allocated capacity */

/* add a new prime to the stored array */
static void add_prime(seq_t p)
{
    if (g_prime_count == g_prime_cap)
    {
        idx_t new_cap = g_prime_cap ? g_prime_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(g_primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        g_primes = tmp;
        g_prime_cap = new_cap;
    }
    g_primes[g_prime_count++] = p;
}

/* trial division on the stored primes */
static int is_prime_trial(seq_t num)
{
    if (num < 2) return 0;
    for (idx_t i = 0; i < g_prime_count; ++i)
    {
        seq_t p = g_primes[i];
        if (p * p > num) break;
        if (num % p == 0) return 0;
    }
    return 1;
}

/* ensure we have at least pos primes (1‑based) cached */
static void ensure_primes(idx_t pos)
{
    if (g_prime_count >= pos) return;

    if (g_prime_count == 0)          /* initialise with the first prime */
    {
        add_prime(2);
    }
    seq_t candidate = g_primes[g_prime_count - 1] + 1;
    if (candidate % 2 == 0) candidate++;  /* keep it odd */

    while (g_prime_count < pos)
    {
        if (is_prime_trial(candidate))
            add_prime(candidate);
        candidate += 2;  /* skip even numbers */
    }
}

/* return the p‑th prime (1‑based)  */
static seq_t get_prime(idx_t p)
{
    ensure_primes(p);
    return g_primes[p - 1];
}

/*----- Main sequence routine ------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range, not used */

    /* Determine the row r and column k */
    idx_t r = 1;
    while ( (r * (r + 1) / 2) < n ) r++;

    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;          /* 1‑based in the row */

    seq_t p_rplus1  = get_prime(r + 1);
    seq_t p_rminuskplus1 = get_prime(r - k + 1);

    return p_rplus1 - p_rminuskplus1;
}
