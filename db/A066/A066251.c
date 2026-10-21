/*
 * A066251  Inverse permutation to A066250.
 * https://oeis.org/A066251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (if n is odd then A000040((n+1)/2) else A002808(n/2)) - 1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy static initialization of prime and composite tables   */
    static int initialized = 0;
    static unsigned *is_prime     = NULL;
    static unsigned *primes_arr   = NULL;
    static unsigned *composites_arr = NULL;
    static idx_t prime_count   = 0;
    static idx_t composite_count = 0;

    if (!initialized) {
        /* LIMIT chosen large enough so that the 6000th prime and
           the 6000th composite are included.  This covers all n ≤ 10000. */
        const idx_t LIMIT = 65000;

        is_prime       = (unsigned *)malloc((LIMIT + 1) * sizeof(unsigned));
        primes_arr     = (unsigned *)malloc(LIMIT * sizeof(unsigned));
        composites_arr = (unsigned *)malloc(LIMIT * sizeof(unsigned));
        if (!is_prime || !primes_arr || !composites_arr) {
            /* Allocation failed – in a real system you'd handle this. */
            return 0;
        }

        /* ---- sieve of Eratosthenes -------------------------------- */
        for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (idx_t p = 2; p * p <= LIMIT; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= LIMIT; j += p) is_prime[j] = 0;
            }
        }

        /* ---- collect primes and composites ------------------------ */
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[i]) {
                primes_arr[prime_count++] = (unsigned)i;
            } else {
                composites_arr[composite_count++] = (unsigned)i; /* i ≥ 4 */
            }
            /* The tables are large enough for all required indices. */
        }

        initialized = 1;
    }

    if ((n & 1U) != 0) {          /* odd n: use primes                     */
        idx_t k = (n + 1) >> 1;    /* 1‑based index of the prime            */
        return (seq_t)(primes_arr[k - 1] - 1);
    } else {                       /* even n: use composite numbers          */
        idx_t k = n >> 1;          /* 1‑based index of the composite number */
        return (seq_t)(composites_arr[k - 1] - 1);
    }
}
