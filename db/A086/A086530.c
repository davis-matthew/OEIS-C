/*
 * A086530  Beginning with 1, odd numbers such that the arithmetic mean of
 *          successive pairs of terms gives primes in their natural order.
 *          {a(n) + a(n+1)}/2 = prime(n).
 * https://oeis.org/A086530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 6493, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086530:  a(1)=1,  a(n+1)=2*prime(n)-a(n)  for n≥1
   (prime(k) is the k‑th prime, prime(1)=2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*-------------------------------------------------------------*/
/* Prime generation utilities (static for incremental use).   */
/*-------------------------------------------------------------*/

static uint64_t *prime_arr      = NULL; /* array of found primes      */
static size_t   prime_cnt      = 0;   /* number of primes stored    */
static size_t   prime_cap      = 0;   /* allocated capacity         */
static uint64_t next_candidate = 3;   /* candidate for next prime   */

/* Ensure that prime_arr contains at least idx primes
   (prime(1) … prime(idx)).  idx is 1‑based. */
static void ensure_primes_up_to_idx(idx_t idx)
{
    if (idx <= prime_cnt)                    /* already have enough */
        return;

    if (!prime_arr) {                        /* initialise first time */
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_arr[0] = 2;
        prime_cnt = 1;
    }

    while (prime_cnt < (size_t)idx) {
        uint64_t c = next_candidate;
        bool is_prime = true;
        for (size_t i = 0;
             i < prime_cnt && prime_arr[i] * prime_arr[i] <= c; ++i)
            if (c % prime_arr[i] == 0) {
                is_prime = false;
                break;
            }

        if (is_prime) {
            if (prime_cnt == prime_cap) {     /* grow the array */
                prime_cap *= 2;
                prime_arr = (uint64_t *)realloc(prime_arr,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = c;
        }
        next_candidate += 2;
    }
}

/*-------------------------------------------------------------*/
/* Sequence function                                         */
/*-------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, avoid UB */
        return (seq_t)0;

    seq_t current = (seq_t)1;          /* a(1) = 1 */

    for (idx_t i = 1; i < n; ++i) {    /* compute a(2) … a(n) */
        ensure_primes_up_to_idx(i);    /* need prime(i) */
        seq_t p = (seq_t)prime_arr[i - 1];   /* prime(i) */
        current = (seq_t)( (seq_t)2 * p - current );
    }

    return current;
}
