/*
 * A141410  Denominator of A027760(n+1)/n.
 * https://oeis.org/A141410
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A141410:  Denominator of A027760(n+1)/n  
   These denominators can be computed without generating the whole  
   sequence A027760 by using the following observation:

   For even n   : a(n) = n/2   (because A027760(n+1)=2)
   For odd  n   : a(n) = n / gcd( n, D(n+1) )
                  where D(k) is the denominator of Bernoulli number B_k.
   Von Staudt–Clausen theorem gives  
      D(2m) = product of all primes p such that p-1 | 2m  (square‑free).
   Consequently for odd n = 2m+1 we have  
      n divides the set of primes {p | p-1 | n+1},  
      and gcd(n, D(n+1)) is the product of the prime divisors of n  
      that satisfy (p-1) | n+1.  
   Therefore

      a(n) = n / ( Π { p | p divides n and (p-1) | n+1 } ).

   The implementation below uses a small sieve to obtain the smallest
   prime factor of any number up to the maximum argument seen so far,
   allowing quick factorisation.  The function works for any unsigned
   64‑bit n.  All arithmetic is performed in the bundle type `seq_t`
   (the caller provides the appropriate typedef).
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t *spf = NULL;      /* smallest prime factor array   */
static idx_t spf_limit = 0;    /* highest index currently stored*/

static void init_spf_to(idx_t limit)
{
    /* extend or build the smallest prime factor table up to limit */
    if (limit <= spf_limit) return;               /* nothing to do */

    idx_t *new_spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!new_spf) return;       /* allocation failed – leave spf unchanged */
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {       /* avoid overflow */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* undefined for n < 2 */

    /* Ensure we can factor numbers up to n */
    init_spf_to(n);

    if ((n & 1) == 0)                     /* even n : a(n)=n/2          */
        return ((seq_t)n) / 2;

    /* odd n : factorise n and accumulate the product of the primes
       that satisfy (p-1) | (n+1)                                    */
    idx_t temp = n;
    idx_t g    = 1;
    while (temp > 1) {
        idx_t p = spf[temp];
        if ((n + 1) % (p - 1) == 0)
            g *= p;
        while (temp % p == 0) temp /= p;
    }

    return ((seq_t)n) / g;
}
