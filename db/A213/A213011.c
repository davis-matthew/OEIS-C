/*
 * A213011  Union of positive squares and sums of first k primes.
 * https://oeis.org/A213011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth element of the sorted union of positive squares
 *            and sums of the first k prime numbers
 * Range:     u8 n <= 26, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool _init_done = false;
static uint32_t * _primes = NULL;
static size_t   _prime_cnt = 0;
static seq_t *  _sum_prefix = NULL;

static void _init_primes(void)
{
    if (_init_done) return;

    /* upper bound for primes – plenty for n up to 10000 */
    const idx_t LIMIT = 500000;

    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* count primes */
    _prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++_prime_cnt;

    _primes = (uint32_t*)malloc(_prime_cnt * sizeof(uint32_t));
    size_t p = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) _primes[p++] = (uint32_t)i;

    free(is_prime);

    /* prefix sums of primes */
    _sum_prefix = (seq_t*)malloc((_prime_cnt + 1) * sizeof(seq_t));
    _sum_prefix[0] = 0;
    for (size_t i = 0; i < _prime_cnt; ++i)
        _sum_prefix[i + 1] = _sum_prefix[i] + (seq_t)_primes[i];

    _init_done = true;
}

seq_t a(idx_t n)
{
    _init_primes();

    seq_t result = 0;
    idx_t count  = 0;

    /* indices into the two sorted sequences */
    idx_t sq_i  = 1;                 /* current square index (i^2)            */
    seq_t sq    = (seq_t)sq_i * sq_i;/* next square value                    */

    idx_t sum_k = 1;                 /* current k for S_k                     */
    seq_t sum   = _sum_prefix[sum_k];/* next prime‑sum value                  */

    while (count < n)
    {
        seq_t next;
        if (sq < sum)
        {
            next = sq;
            ++sq_i;
            sq   = (seq_t)sq_i * sq_i;
        }
        else if (sum < sq)
        {
            next = sum;
            ++sum_k;
            sum  = _sum_prefix[sum_k];
        }
        else /* equal – keep the value once and advance both sequences */
        {
            next = sq;
            ++sq_i;
            sq   = (seq_t)sq_i * sq_i;
            ++sum_k;
            sum  = _sum_prefix[sum_k];
        }

        result = next;
        ++count;
    }

    return result;
}
