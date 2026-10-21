/*
 * A079063  Least k such that sqrt(prime(n+k))-sqrt(prime(n))>1.
 * https://oeis.org/A079063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that sqrt(p_{n+k}) - sqrt(p_n) > 1
 * Range:     u8 n <= 54, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here p_m denotes the m-th prime (p_1 = 2).  The implementation
   precomputes all primes up to a fixed limit (200000) which
   is sufficient for n up to about 10000.  The test
   sqrt(p_{n+k}) > sqrt(p_n) + 1 is checked using integer
   arithmetic only:
        let diff = p_{n+k} - p_n - 1
        if diff > 0 and diff*diff > 4*p_n then the inequality holds.
   That is equivalent to sqrt(p_{n+k}) > sqrt(p_n)+1 without
   using floating‑point operations.                                 */
#include <stdbool.h>
#include <stddef.h>

#define LIMIT 200000  /* enough to cover primes for n <= 10000 */

static bool is_prime_arr[LIMIT + 1];
static idx_t primes_arr[LIMIT];
static size_t prime_cnt = 0;
static bool initialized = false;

static void init_primes(void)
{
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime_arr[i] = true;
    is_prime_arr[0] = false;
    is_prime_arr[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) {
                is_prime_arr[j] = false;
            }
        }
    }
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            primes_arr[prime_cnt++] = i;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_primes();

    if (n == 0) return 0; /* undefined, but return 0 for safety */

    idx_t idx0 = n - 1;          /* zero–based index of p_n */
    if (idx0 >= prime_cnt) return 0;  /* out of precomputed range */

    seq_t p_n = (seq_t)primes_arr[idx0];

    for (size_t k = 1; ; ++k) {
        idx_t idxk = idx0 + k;
        if (idxk >= prime_cnt) break;  /* should never happen for n <= 10000 */

        seq_t p_nk = (seq_t)primes_arr[idxk];
        seq_t diff = p_nk - p_n - 1;
        if (diff > 0 && diff * diff > 4 * p_n) {
            return (seq_t)k;
        }
    }
    return 0; /* unreachable for valid n */
}
