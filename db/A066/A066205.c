/*
 * A066205  a(n) = Product_{k=1..n} prime(2k-1), where prime(k) is k-th prime.
 * https://oeis.org/A066205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} prime(2k‑1) (product of the first n
 *            odd‑indexed primes)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Offset is 1; undefined behaviour for n == 0 – return 0 safely. */
    if (n == 0) return 0;

    /* Number of odd‑indexed primes required. */
    idx_t need = 2 * n - 1;            /* prime position of the last needed odd index */

    /* Estimate an upper bound for the need‑th prime.
       For need ≥ 6, p_ⱼ ≤ n(ln n + ln ln n).  For smaller values a very small
       fixed bound suffices. */
    idx_t limit;
    if (need < 6) {
        limit = 15;                    /* 5th prime is 11, 6th is 13 */
    } else {
        double nd = (double)need;
        double bound = nd * (log(nd) + log(log(nd)));
        limit = (idx_t)(bound) + 3;    /* add a tiny safety margin */
    }

    /* Simple sieve up to 'limit'. */
    size_t len = (size_t)(limit + 1);
    /* Use a byte array; 0 = prime, 1 = composite. */
    char *sieve = (char *)calloc(len, sizeof(char));
    if (!sieve) return 0;              /* allocation failure – unlikely */

    seq_t prod = 1;
    idx_t prime_idx = 0;                /* 1‑based count of primes found */

    for (idx_t num = 2; num <= limit; ++num) {
        if (!sieve[num]) {              /* num is prime */
            ++prime_idx;
            /* Multiply only odd‑indexed primes (prime_idx == 1,3,5,...) */
            if ((prime_idx & 1) == 1) {
                prod *= (seq_t)num;
            }
            /* Mark multiples of this prime, avoiding overflow of num*num. */
            if (num <= limit / num) {
                for (idx_t j = num * num; j <= limit; j += num) {
                    sieve[j] = 1;
                }
            }
            /* Stop once we have reached the required number of primes. */
            if (prime_idx >= need) break;
        }
    }

    free(sieve);
    return prod;
}
