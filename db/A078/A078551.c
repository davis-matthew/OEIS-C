/*
 * A078551  Largest prime dividing sigma(2,n).
 * https://oeis.org/A078551
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of σ₂(n) = sum_{d|n} d² (a(n) =
 *            A006530(A001157(n)) as stated in the OEIS entry)
 * Range:     u8 n <= 28, u16 n <= 378, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int init_prime_sieve(uint32_t *primes, size_t *cnt, uint32_t limit)
{
    uint8_t *is_composite = (uint8_t *)malloc(limit + 1);
    if (!is_composite) return 0;
    for (uint32_t i = 2; i <= limit; ++i) is_composite[i] = 0;
    size_t index = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[index++] = i;
            if ((uint64_t)i * i <= limit)
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
        }
    }
    *cnt = index;
    free(is_composite);
    return 1;
}

/* static storage for primes up to 200000 */
static uint32_t SMALL_PRIMES[200000];
static size_t SMALL_PRIMES_COUNT = 0;
static int PRIMES_INITIALIZED = 0;

/* factor n into (p_i, e_i). Returns number of distinct factors.  */
static size_t factor_uint64(uint64_t n, uint64_t *pr, uint8_t *exp)
{
    size_t cnt = 0;
    for (size_t i = 0; i < SMALL_PRIMES_COUNT; ++i) {
        uint32_t p = SMALL_PRIMES[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            uint8_t c = 0;
            while (n % p == 0) {
                n /= p;
                ++c;
            }
            pr[cnt] = p;
            exp[cnt] = c;
            ++cnt;
        }
    }
    if (n > 1) {
        pr[cnt] = n;
        exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* lazy initialization of prime list */
    if (!PRIMES_INITIALIZED) {
        init_prime_sieve(SMALL_PRIMES, &SMALL_PRIMES_COUNT, 200000);
        PRIMES_INITIALIZED = 1;
    }
    if (n < 2) return (seq_t)0;   /* undefined below offset */

    /* factor n */
    uint64_t primes_arr[64];
    uint8_t exps_arr[64];
    size_t fac_cnt = factor_uint64((uint64_t)n, primes_arr, exps_arr);

    uint64_t max_prime_factor = 2;  /* at least 2 for n>=2 */

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes_arr[i];
        uint8_t e = exps_arr[i];

        /* compute sigma_p = sum_{k=0..e} p^(2k) */
        uint64_t sum = 1;
        uint64_t term = 1;
        uint64_t p2 = p * p;
        for (uint8_t k = 1; k <= e; ++k) {
            term *= p2;
            sum += term;
        }
        /* factor sum to update max_prime_factor */
        uint64_t m = sum;
        for (size_t j = 0; j < SMALL_PRIMES_COUNT; ++j) {
            uint32_t q = SMALL_PRIMES[j];
            if ((uint64_t)q * q > m) break;
            if (m % q == 0) {
                if (q > max_prime_factor) max_prime_factor = q;
                while (m % q == 0) m /= q;
            }
        }
        if (m > 1) {
            if (m > max_prime_factor) max_prime_factor = m;
        }
    }

    return (seq_t)max_prime_factor;
}
