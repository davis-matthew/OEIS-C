/*
 * A089759  Table T(n,k), 0<=k, 0<=n, read by antidiagonals, defined by T(n,k)
 *          = (k*n)! / (n!)^k.
 * https://oeis.org/A089759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 29, u32 n <= 47, u64 n <= 69, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A089759 – lattice path counts read by antidiagonals.
   For an index n>=0, let s be the unique integer with
      n < (s+1)(s+2)/2   and   n >= s(s+1)/2
   Then let n' = n - s(s+1)/2   and   k' = s - n'.
   The term is 
      a(n) = T(n',k') = (k'n')! / (n'!)^k' .
   The function computes this value exactly using prime‑exponent
   factorisation, which keeps all intermediate results within
   the limits of seq_t.
*/

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Prime storage */
static int  primes[3000];
static size_t prime_count = 0;
static void init_primes(void)
{
    if (prime_count != 0) return;          /* already initialised */

    const int LIMIT = 20000;               /* sufficient for n up to 10000 */
    unsigned char is_prime[LIMIT + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = i;
}

/* exponent of prime p in m! */
static inline idx_t fact_exp(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m)
    {
        m /= p;
        e += m;
    }
    return e;
}

/* fast exponentiation (seq_t base ^ exp) */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* T(0,0) */

    init_primes();                         /* initialise prime table */

    /* find antidiagonal s */
    idx_t s = 0;
    while (n >= (s + 1) * (s + 2) / 2) ++s;

    idx_t prev_sum = s * (s + 1) / 2;      /* elements of earlier diagonals */
    idx_t pos_in_diag = n - prev_sum;     /* position within current diagonal */
    idx_t nval = pos_in_diag;              /* first component of T */
    idx_t kval = s - pos_in_diag;          /* second component of T */

    idx_t max = nval * kval;               /* (n*k) value for factorial */
    seq_t result = 1;

    /* factorise (k*n)! and divide by (n!)^k using prime exponents */
    for (idx_t i = 0; i < prime_count && primes[i] <= max; ++i)
    {
        idx_t p = primes[i];
        idx_t exp = fact_exp(max, p) - kval * fact_exp(nval, p);
        if (exp > 0)
            result *= pow_uint(p, exp);
    }

    return result;
}
