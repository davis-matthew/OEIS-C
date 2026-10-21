/*
 * A095750  "Degree" of the Sophie Germain primes (A005384).
 * https://oeis.org/A095750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A095750 – Degree of Sophie Germain primes.
 *
 *  For a Sophie Germain prime p (p and 2p+1 are prime) define
 *
 *      deg(2)       = 0
 *      deg(p) = deg((p-1)/2) + 1   if (p-1)/2 is also a SG prime
 *      deg(p) = 0                 otherwise
 *
 *  The sequence a(n) is the degree of the n‑th SG prime (a(0) = 0),
 *  where the SG primes are taken in increasing order.
 *  The routine keeps a static sieve and recomputes it only when
 *  more SG primes are required.  The algorithm is
 *  O(limit log log limit) where limit \~ 2·p_n (p_n = n‑th SG prime).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  utility:  sieve of Eratosthenes –  is_prime[i] = 1 if i is prime   */
static void sieve_primes(idx_t limit, uint8_t *is_prime)
{
    /*  initialise all as prime  */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
}

/*  the main routine  */
seq_t a(idx_t n)
{
    static idx_t  sieve_limit = 0;          /* highest number sieved   */
    static idx_t  sg_count   = 0;          /* number of SG primes found */
    static seq_t *sg_degrees = NULL;       /* degrees of SG primes in order */

    /*  enlarge sieve until we have at least n+1 SG primes   */
    while (sg_count <= n) {
        /*  choose a new limit – double each time; start with 1000   */
        idx_t new_limit = (sieve_limit == 0) ? 1000 : sieve_limit * 2;

        /*  buffers for the new sieve   */
        uint8_t *is_prime = (uint8_t*)calloc((size_t)new_limit + 1, 1);
        if (!is_prime) return 0;   /* out of memory – give up with 0  */

        /*  sieve primes up to new_limit   */
        sieve_primes(new_limit, is_prime);

        /*  array to hold SG flag and degree per prime value   */
        uint8_t *is_sg  = (uint8_t*)calloc((size_t)new_limit + 1, 1);
        seq_t  *degree = (seq_t*)calloc((size_t)new_limit + 1, sizeof(seq_t));
        if (!is_sg || !degree) {
            free(is_prime);  free(is_sg);  free(degree);
            return 0;
        }

        /*  find SG primes and compute their degrees   */
        idx_t new_count = 0;
        seq_t *new_deg_list = NULL;   /* will allocate after knowing count */

        /*  first pass: mark SG flags   */
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (!is_prime[p]) continue;
            idx_t twin = 2 * p + 1;
            if (twin <= new_limit && is_prime[twin])
                is_sg[p] = 1;
        }

        /*  second pass: compute degrees in increasing order   */
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (!is_sg[p]) continue;

            seq_t d = 0;
            idx_t pred = (p - 1) / 2;
            if (pred >= 2 && pred <= new_limit && is_sg[pred]) {
                d = degree[pred] + 1;
            }
            degree[p] = d;
            new_count++;
        }

        /*  allocate the degree list according to new_count   */
        new_deg_list = (seq_t*)calloc((size_t)new_count, sizeof(seq_t));
        if (!new_deg_list) {
            free(is_prime); free(is_sg); free(degree);
            return 0;
        }

        idx_t pos = 0;
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (!is_sg[p]) continue;
            new_deg_list[pos++] = degree[p];
        }

        /*  free old data  */
        free(is_prime); free(is_sg); free(degree); free(sg_degrees);

        /*  store new data   */
        sg_degrees = new_deg_list;
        sg_count   = new_count;
        sieve_limit = new_limit;
    }

    /*  at this point sg_degrees[n] exists   */
    return sg_degrees[n];
}
