/*
 * A074975  Abundant numbers such that the sum of their individual digits when
 *          raised to their own power is an abundant number.
 * https://oeis.org/A074975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A074975
   The n-th abundant number whose digits, each raised to their own
   power and summed, produce another abundant number.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static const uint64_t LIMIT = 2000000;           /* pre‑compute σ up to LIMIT   */
static uint64_t *sigma_array = 0;                /* σ[1..LIMIT]                 */
static seq_t cache_terms[10001];                /* store first 10000 terms      */
static size_t cache_len = 0;

/* pre‑computed primes up to sqrt(8e9) ≈ 100000 */
static uint32_t *primes = 0;
static size_t primes_cnt = 0;

/* d^d with 0^0 = 1 */
static seq_t digit_pow[10];

/* ------------------------------------------------------------ */
/* Utility to build σ[n] for 1 ≤ n ≤ LIMIT                          */
static void build_sigma(void)
{
    sigma_array = (uint64_t*)malloc((LIMIT + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= LIMIT; ++i) sigma_array[i] = 0;
    for (uint64_t i = 1; i <= LIMIT; ++i)
        for (uint64_t j = i; j <= LIMIT; j += i)
            sigma_array[j] += i;
}

/* ------------------------------------------------------------ */
/* Build primes up to 100000                                    */
static void build_primes(void)
{
    const int N = 100000;
    bool *is_composite = (bool*)calloc(N + 1, sizeof(bool));
    for (int i = 2; i <= N; ++i) {
        if (!is_composite[i]) {
            primes_cnt++;
        }
    }
    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (int i = 2; i <= N; ++i) {
        if (!is_composite[i]) {
            primes[idx++] = (uint32_t)i;
            if ((int64_t)i * i <= N) {
                for (int j = i * i; j <= N; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ------------------------------------------------------------ */
/* Pre‑compute digit powers: 0^0 = 1                            */
static void build_digit_pow(void)
{
    digit_pow[0] = (seq_t)1;
    for (int d = 1; d <= 9; ++d) {
        seq_t val = 1;
        for (int i = 0; i < d; ++i)
            val *= (seq_t)d;
        digit_pow[d] = val;
    }
}

/* ------------------------------------------------------------ */
/* Check if n (uint64_t) is abundant by using precomputed σ    */
static inline bool abundant_uint64(uint64_t n)
{
    return sigma_array[n] > 2 * n;
}

/* ------------------------------------------------------------ */
/* Check if x (seq_t) is abundant – factorise |x|                */
static bool abundant_seq_t(seq_t x)
{
    uint64_t n = (uint64_t)x;          /* fits in 64‑bit for this problem */
    if (n < 12) return false;         /* smallest abundant number */ 

    unsigned __int128 sum = 1;
    uint64_t m = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > m) break;
        if (m % p == 0) {
            unsigned __int128 term = 1;
            unsigned __int128 p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= p;
                term += p_pow;
            }
            sum *= term;
        }
    }
    if (m > 1) sum *= (unsigned __int128)(1 + m);
    return sum > (unsigned __int128)2 * n;
}

/* ------------------------------------------------------------ */
/* Initialise once                              */
static void init_once(void)
{
    if (initialized) return;
    build_sigma();
    build_primes();
    build_digit_pow();
    initialized = true;
}

/* ------------------------------------------------------------ */
/*        a(idx_t n) – the requested term                    */
seq_t a(idx_t n)
{
    init_once();
    /* grow cache until we have enough terms */
    static uint64_t current = 0;
    while (cache_len < n) {
        ++current;
        if (!abundant_uint64(current)) continue;
        uint64_t tmp = current;
        seq_t s = 0;
        while (tmp > 0) {
            int d = tmp % 10;
            s += digit_pow[d];
            tmp /= 10;
        }
        /* for numbers <10, the loop above handles the single digit,
           including d=0 (if current==0 but we never reach it). */
        if (abundant_seq_t(s)) {
            cache_terms[cache_len++] = (seq_t)current;
        }
    }
    return cache_terms[n - 1];
}
