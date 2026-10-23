/*
 * A025100  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (1, p(1), p(2), ...), t = (composite numbers).
 * https://oeis.org/A025100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} s(i) * t(n-i+1) with s(1) = 1, s(i) =
 *            p(i-1) for i≥2 (p(k) = k‑th prime) t(1),t(2),… are the composite
 *            numbers 6,8,9,10,12,… (4 is skipped)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/*------------------------------------------------------------------*/
/*  Cached data structures                                          */
/*------------------------------------------------------------------*/
static size_t  cached_limit      = 0;          /* sieve limit used       */
static uint64_t *prime_list      = NULL;       /* primes ≥2, in order    */
static size_t   prime_count      = 0;           /* number of primes       */
static uint64_t *composite_list  = NULL;       /* composites ≥6, in order*/
static size_t   composite_count  = 0;           /* number of composites   */

/*------------------------------------------------------------------*/
/*  Rebuild the prime and composite tables as needed                */
/*------------------------------------------------------------------*/
static void rebuild_tables(idx_t need)
/* Build tables sufficient for computations with index 'need'.
   The sieve limit is chosen as 2*need+100 which is enough for
   the first 'need' composites when 'need' ≤ 10000.          */
{
    /* If the cached tables are already sufficient, do nothing.       */
    idx_t primes_needed = (need + 1) / 2;                 /* k-1 primes needed */
    if (need <= composite_count && primes_needed <= prime_count + 1)
        return;

    /* Free any previously allocated memory.                         */
    free(prime_list);
    free(composite_list);
    prime_list     = NULL;
    composite_list = NULL;
    prime_count    = 0;
    composite_count= 0;
    cached_limit   = 0;

    /* Choose sieve limit.                                            */
    size_t limit = (size_t)need * 2 + 100;
    if (limit < 6) limit = 6;                     /* ensure we can store 6 */

    /* Sieve of Eratosthenes.                                         */
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;                       /* allocation failed */

    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = false;
        }
    }

    /* Collect primes.                                                 */
    size_t pr_cap = limit / 10 + 1;
    prime_list = (uint64_t*)malloc(pr_cap * sizeof(uint64_t));
    if (!prime_list) { free(is_prime); return; }
    size_t pc = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (pc == pr_cap) {
                pr_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, pr_cap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(prime_list); return; }
                prime_list = tmp;
            }
            prime_list[pc++] = (uint64_t)i;
        }
    }
    prime_count = pc;

    /* Collect composites ≥6.                                          */
    size_t comp_cap = limit / 2 + 1;
    composite_list = (uint64_t*)malloc(comp_cap * sizeof(uint64_t));
    if (!composite_list) { free(is_prime); free(prime_list); return; }
    size_t cc = 0;
    for (size_t i = 6; i <= limit; ++i) {
        if (!is_prime[i]) {
            if (cc == comp_cap) {
                comp_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(composite_list, comp_cap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(prime_list); free(composite_list); return; }
                composite_list = tmp;
            }
            composite_list[cc++] = (uint64_t)i;
        }
    }
    composite_count = cc;

    free(is_prime);
    cached_limit = limit;                         /* tables built up to 'limit' */
}

/*------------------------------------------------------------------*/
/*  Sequence function                                               */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
/* Compute a(n) for n ≥ 1 according to the definition above.       */
{
    if (n == 0) return (seq_t)0;                 /* undefined, return 0 */

    rebuild_tables(n);                           /* ensure tables ready */

    idx_t k = (n + 1) / 2;                       /* floor((n+1)/2) */
    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t s_val = (i == 1) ? (seq_t)1
                               : (seq_t)prime_list[i - 2];
        seq_t t_val = (seq_t)composite_list[n - i]; /* t(n-i+1) */
        sum += s_val * t_val;
    }

    return sum;
}
