/*
 * A103558  Semiprimes of the form p^2 + q^2, where p and q are primes.
 * https://oeis.org/A103558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A103558: semiprimes of the form p^2 + q^2 where p and q are distinct primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*---------- Helper: simple sieve of Eratosthenes --------------------------------*/
static void simple_sieve(uint32_t limit, uint32_t **primes_out, size_t *count_out)
{
    uint8_t *is_prime = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes first to allocate exact memory */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *pr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!pr) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = i;

    free(is_prime);
    *primes_out = pr;
    *count_out  = cnt;
}

/*---------- Comparator for qsort ---------------------------------------------*/
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t*)a;
    seq_t y = *(const seq_t*)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

/*---------- Semiprime test using the prime table -----------------------------*/
static bool is_semiprime_uint64(uint64_t n, const uint32_t *primes, size_t prime_cnt)
{
    if (n < 6) return false;      /* smallest semiprime is 4, but n won't be that small */
    int total = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i)
    {
        uint32_t p = primes[i];
        while (n % p == 0)
        {
            ++total;
            n /= p;
            if (total > 2) return false;
        }
    }
    if (n > 1) ++total;
    return total == 2;
}

/*---------- Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* Static storage */
    static seq_t  *seq = NULL;    /* sorted list of terms */
    static idx_t   seq_size = 0;
    static bool    built = false;

    if (!built)
    {
        /* Build the sequence once, on the first call. */

        /* Prime sieve parameters */
        const uint32_t SIEVE_LIMIT = 6000;   /* primes up to this for factorisation */
        uint32_t *prime_list = NULL;
        size_t prime_cnt = 0;

        simple_sieve(SIEVE_LIMIT, &prime_list, &prime_cnt);

        /* Determine the upper bound for sums:
           we will generate sums for all prime pairs with p < q <= 3000.
           3000 ensures at least 10 000 terms while keeping the array size small. */
        const uint32_t PRIME_FILTER_LIMIT = 3000;
        size_t prime_filter_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i)
            if (prime_list[i] <= PRIME_FILTER_LIMIT)
                ++prime_filter_cnt;

        /* Allocate buffer for all candidate sums.  The number of unordered
           pairs (p < q) is prime_filter_cnt*(prime_filter_cnt-1)/2. */
        size_t pair_cap = prime_filter_cnt * (prime_filter_cnt - 1) / 2;
        seq_t *sums = (seq_t*)malloc(pair_cap * sizeof(seq_t));
        if (!sums) exit(EXIT_FAILURE);
        size_t sums_size = 0;

        /* Enumerate unordered pairs and keep only semiprime sums. */
        for (size_t i = 0; i < prime_filter_cnt; ++i)
        {
            uint64_t p = prime_list[i];
            uint64_t p2 = p * p;
            for (size_t j = i + 1; j < prime_filter_cnt; ++j)
            {
                uint64_t q = prime_list[j];
                uint64_t sum = p2 + q * q;      /* p^2 + q^2 */
                if (is_semiprime_uint64(sum, prime_list, prime_cnt))
                    sums[sums_size++] = (seq_t)sum;
            }
        }

        free(prime_list);   /* prime list no longer needed */

        /* Sort the sums and remove duplicates */
        if (sums_size > 1)
            qsort(sums, sums_size, sizeof(seq_t), cmp_seq);

        /* Deduplicate in place */
        size_t uniq = 0;
        for (size_t i = 0; i < sums_size; ++i)
        {
            if (uniq == 0 || sums[i] != sums[uniq - 1])
                sums[uniq++] = sums[i];
        }
        sums = (seq_t*)realloc(sums, uniq * sizeof(seq_t));   /* shrink to used size */

        seq        = sums;
        seq_size   = uniq;
        built      = true;
    }

    /* Return the n-th term (1‑based).  Out‑of‑range requests return 0. */
    if (n == 0 || n > seq_size) return 0;
    return seq[n - 1];
}
