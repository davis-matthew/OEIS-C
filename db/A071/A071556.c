/*
 * A071556  Smallest x > 1 such that x^prime(n) == 1 mod(prime(i)) 4<=i<=n.
 * https://oeis.org/A071556
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + ∏_{i=4}^{n} prime(i) (prime(i) = i‑th prime, with
 *            p1=2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* static storage for primes and the corresponding prefix products   */
static uint64_t *primes  = NULL;  /* primes[i] = (i+1)-th prime       */
static size_t   primes_size = 0;  /* number of primes stored          */
static size_t   primes_cap  = 0;  /* allocated capacity of primes      */

static seq_t *prod_arr = NULL;    /* prod_arr[i] = product of primes[3..i]   */
static size_t   prod_cap  = 0;     /* allocated capacity of prod_arr         */

/* Expand the prime list and product array to contain at least `need` primes   */
static void ensure_primes(idx_t need)
{
    if (primes_size >= (size_t)need) return;

    /* enlarge primes array */
    if (primes_cap < (size_t)need) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 32;
        if (new_cap < (size_t)need) new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) return;                     /* out of memory – abort silently */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* enlarge product array */
    if (prod_cap < (size_t)need) {
        size_t new_cap = prod_cap ? prod_cap << 1 : 32;
        if (new_cap < (size_t)need) new_cap = need;
        seq_t *tmp2 = (seq_t *)realloc(prod_arr, new_cap * sizeof(seq_t));
        if (!tmp2) return;                     /* out of memory – abort silently */
        prod_arr = tmp2;
        prod_cap = new_cap;
    }

    /* generate additional primes, and fill prefix products   */
    uint64_t candidate = (primes_size ? primes[primes_size-1] + 1 : 2);

    while (primes_size < (size_t)need) {
        /* test whether candidate is prime */
        int is_prime = 1;
        uint64_t root = (uint64_t)sqrt((double)candidate);
        for (size_t j = 0; j < primes_size && primes[j] <= root; ++j) {
            if (candidate % primes[j] == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            size_t idx = primes_size;          /* index of the new prime    */
            /* update prefix products for indices >=3   */
            if (idx == 3) {
                prod_arr[3] = (seq_t)candidate;          /* p4 */
            } else if (idx >= 4) {
                prod_arr[idx] = prod_arr[idx-1] * (seq_t)candidate;
            }
            primes[primes_size++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                         /* undefined for n < 4      */
    ensure_primes(n);                            /* guarantee we have primes */
    seq_t res = prod_arr[(size_t)n-1];           /* product of primes[3..n-1] */
    res += (seq_t)1;
    return res;
}
