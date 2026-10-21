/*
 * A069458  Twin pack primes.
 * https://oeis.org/A069458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Twin pack primes: a(n) is the n‑th prime that belongs to a block of
   consecutive primes containing at least two twin prime pairs with no
   other primes between those pairs. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------
 * Sieve of Eratosthenes – generate all primes ≤ limit
 * -------------------------------------------------------------------------- */
static void sieve_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) abort();

    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; (idx_t)p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!arr) abort();
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) arr[k++] = (uint64_t)i;

    free(is_prime);
    *primes_out = arr;
    *cnt_out     = cnt;
}

/* --------------------------------------------------------------------------
 * Global state for twin‑pack prime list
 * -------------------------------------------------------------------------- */
static seq_t *twin_list  = NULL;
static size_t twin_count = 0;
static size_t twin_cap   = 0;
static idx_t  sieve_limit = 0;

/* --------------------------------------------------------------------------
 * Rebuild twin‑pack primes up to the given limit
 * -------------------------------------------------------------------------- */
static void recompute_twin_pack(idx_t limit)
{
    /* Obtain all primes up to limit */
    uint64_t *primes = NULL;
    size_t   prime_cnt = 0;
    sieve_primes(limit, &primes, &prime_cnt);

    /* Allocate temporary storage for twin‑pack primes */
    seq_t *tmp = NULL;
    size_t tmp_cnt = 0;
    size_t tmp_cap = 0;

    /* Boolean arrays: prime is part of a twin pair? and does a twin pair start here? */
    bool *flag  = (bool *)malloc(prime_cnt);
    bool *start = (bool *)malloc(prime_cnt);
    if (!flag || !start) abort();

    for (size_t i = 0; i < prime_cnt; ++i) {
        bool twin = false;
        if (i > 0 && primes[i] - primes[i - 1] == 2ULL) twin = true;
        if (i + 1 < prime_cnt && primes[i + 1] - primes[i] == 2ULL) twin = true;
        flag[i]  = twin;
        start[i] = (i + 1 < prime_cnt && primes[i + 1] - primes[i] == 2ULL);
    }

    /* Scan for maximal blocks of primes all belonging to at least one twin pair */
    size_t i = 0;
    while (i < prime_cnt) {
        /* Skip primes that are not in any twin pair */
        if (!flag[i]) { ++i; continue; }

        size_t block_start = i;
        size_t twins = start[i] ? 1 : 0;
        size_t j = i + 1;
        while (j < prime_cnt && flag[j]) {
            if (start[j]) ++twins;
            ++j;
        }
        size_t block_end = j - 1;

        /* If the block contains at least two twin pairs, store its primes */
        if (twins >= 2) {
            for (size_t k = block_start; k <= block_end; ++k) {
                if (tmp_cnt == tmp_cap) {
                    size_t new_cap = (tmp_cap == 0) ? 16 : (tmp_cap << 1);
                    seq_t *new_pl = (seq_t *)realloc(tmp, new_cap * sizeof(seq_t));
                    if (!new_pl) abort();
                    tmp = new_pl;
                    tmp_cap = new_cap;
                }
                tmp[tmp_cnt++] = (seq_t)primes[k];
            }
        }

        i = j;   /* continue after this block */
    }

    free(primes);
    free(flag);
    free(start);

    /* Replace the old global list */
    free(twin_list);
    twin_list  = tmp;
    twin_count = tmp_cnt;
    twin_cap   = tmp_cap;
    sieve_limit = limit;
}

/* --------------------------------------------------------------------------
 * Ensure at least n twin‑pack primes are available
 * -------------------------------------------------------------------------- */
static void ensure_n(idx_t n)
{
    if (twin_count >= n) return;

    idx_t limit = (sieve_limit ? sieve_limit << 1 : 200000);
    while (twin_count < n) {
        recompute_twin_pack(limit);
        if (twin_count >= n) break;
        if (limit < 1000000) limit <<= 1;
        else                 limit += 1000000;
    }
}

/* --------------------------------------------------------------------------
 * Public function: return a(n)
 * -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_n(n);
    return twin_list[n - 1];
}
