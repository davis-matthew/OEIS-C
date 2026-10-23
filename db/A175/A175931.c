/*
 * A175931  Numbers n such that A129363(n-1) = A129363(n) = A129363(n+1).
 * https://oeis.org/A175931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175931: a(n) = the nth integer m such that
 * A129363(m-1) = A129363(m) = A129363(m+1).
 * The function builds the required sequence once by
 * sieving twin primes up to the needed bound and
 * caching the results.  Subsequent calls are O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache for the sequence values */
static seq_t *a_list   = NULL;
static size_t a_len    = 0;
static bool   initialized = false;

/* Upper bound for n needed during precomputation:
 * the largest known term is 2103; we allow a small margin. */
static const idx_t MAX_N   = 26001;                /* maximum n to examine   */
static const idx_t LIMIT   = 2 * MAX_N + 4;        /* primes up to 2*MAX_N+2 */

/* one‑time initialization routine */
static void init(void)
{
    /* --- sieve for primes up to LIMIT ----------------------------------- */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) return;                      /* allocation failed */
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* --- build list of primes ------------------------------------------ */
    size_t prime_est = LIMIT / 10 + 10;
    uint32_t *primes = (uint32_t *)malloc(prime_est * sizeof(uint32_t));
    size_t    prime_cnt = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (is_prime[p]) {
            if (prime_cnt == prime_est) {
                prime_est <<= 1;
                primes = (uint32_t *)realloc(primes, prime_est * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)p;
        }
    }

    /* --- collect twin primes ------------------------------------------- */
    size_t twin_est = prime_cnt / 2 + 10;
    uint32_t *twin = (uint32_t *)malloc(twin_est * sizeof(uint32_t));
    size_t    twin_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        /* a prime is a twin prime if p-2 or p+2 is prime */
        if ((p >= 3 && is_prime[p-2]) || (p + 2 <= LIMIT && is_prime[p+2])) {
            if (twin_cnt == twin_est) {
                twin_est <<= 1;
                twin = (uint32_t *)realloc(twin, twin_est * sizeof(uint32_t));
            }
            twin[twin_cnt++] = p;
        }
    }

    /* --- count partitions of 2*k into two (unordered) twin primes ---- */
    const size_t MAX_SUM = 2 * MAX_N + 2;                 /* maximum 2*k */
    uint32_t *cnt_sum = (uint32_t *)calloc(MAX_SUM + 1, sizeof(uint32_t));

    for (size_t i = 0; i < twin_cnt; ++i) {
        for (size_t j = i; j < twin_cnt; ++j) {
            uint32_t sum = twin[i] + twin[j];
            if (sum > MAX_SUM) break;
            ++cnt_sum[sum];
        }
    }

    /* --- build the sequence list a_list -------------------------------- */
    for (idx_t k = 2; k <= MAX_N; ++k) {          /* start at 2 to avoid n=1 */
        uint32_t c_prev = cnt_sum[2 * (k - 1)];
        uint32_t c_cur  = cnt_sum[2 * k];
        uint32_t c_next = cnt_sum[2 * (k + 1)];
        if (c_prev == c_cur && c_cur == c_next) {
            a_list = (seq_t *)realloc(a_list, (a_len + 1) * sizeof(seq_t));
            a_list[a_len++] = (seq_t)k;
        }
    }

    /* --- clean up ------------------------------------------------------- */
    free(is_prime);
    free(primes);
    free(twin);
    free(cnt_sum);

    initialized = true;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n == 0) return (seq_t)0;                /* undefined below offset */
    if ((size_t)(n - 1) < a_len) return a_list[n - 1];
    return (seq_t)0;                            /* out of range */
}
