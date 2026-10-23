/*
 * A023559  Convolution of A023531 and (1, p(1), p(2), ...).
 * https://oeis.org/A023559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=2}^{⌊(√(8(n+1)+1)-1)/2⌋} [ (n - T_m + 2 == 1) ? 1 :
 *            prime(n - T_m + 1) ] where T_m = m(m+1)/2 and prime(k) is the
 *            k‑th prime (prime(1)=2)
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Static storage for the list of primes; primes[i] holds the (i+1)-th prime. */
static uint64_t *primes = NULL;   /* array of primes */
static size_t prime_cnt = 0;      /* number of primes stored */

/* Allocate and generate primes up to 'limit' (inclusive).  Existing prime
   data is freed.  The new list will contain all primes ≤ limit. */
static void generate_primes_upto(idx_t limit)
{
    if (limit < 2) {
        free(primes);
        primes = NULL;
        prime_cnt = 0;
        return;
    }

    /* Boolean array for the sieve */
    size_t sieve_sz = (size_t)limit + 1;
    uint8_t *is_composite = (uint8_t *)malloc(sieve_sz);
    if (!is_composite) abort();
    memset(is_composite, 0, sieve_sz);

    free(primes);
    primes = NULL;
    prime_cnt = 0;

    for (idx_t num = 2; num <= limit; ++num) {
        if (!is_composite[num]) {
            /* Found a prime.  Append to the list. */
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                (prime_cnt + 1) * sizeof(uint64_t));
            if (!tmp) abort();
            primes = tmp;
            primes[prime_cnt++] = (uint64_t)num;

            /* Cross out multiples of the prime.  The starting point
               num * num is safe because num² ≤ limit. */
            if (num <= limit / num) {
                for (idx_t mult = num * num; mult <= limit; mult += num)
                    is_composite[mult] = 1;
            }
        }
    }

    free(is_composite);
}

/* Ensure that the prime list contains at least 'needed' primes.
   If not, regenerate with a larger sieve limit. */
static void ensure_primes_up_to(idx_t needed)
{
    if (prime_cnt >= needed) return;

    /* Rough upper bound for the n-th prime (Rosser's theorem).  For
       n ≤ 1e6 this bound is safe, and here n ≤ 1e4. */
    idx_t limit = (idx_t)(needed * 15) + 100;
    if (limit < 1000) limit = 1000;

    while (1) {
        generate_primes_upto(limit);
        if (prime_cnt >= needed) break;
        limit *= 2;          /* increase limit and retry if needed */
    }
}

/* --- Public sequence function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset; return 0 */

    seq_t sum = 0;
    idx_t n_plus_one = n + 1;          /* value used in the triangular test */

    /* Iterate over m starting from 2 (since T_1 gives index 0). */
    for (idx_t m = 2;; ++m) {
        idx_t t = m * (m + 1) / 2;     /* T_m */
        if (t > n_plus_one) break;     /* stop when triangular exceeds n+1 */

        idx_t pos = n - t + 2;         /* position in the (1, p(1), p(2), ...) sequence */

        if (pos == 1) {
            sum += (seq_t)1;           /* g(1) = 1 */
        } else {
            /* we need the (pos-1)-th prime */
            idx_t prime_index = pos - 1;                 /* 1‑based index of the prime */
            ensure_primes_up_to(prime_index);            /* guarantee enough primes */
            seq_t prime_val = (seq_t)primes[prime_index - 1];
            sum += prime_val;
        }
    }

    return sum;
}
