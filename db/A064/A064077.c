/*
 * A064077  Greater of odd twin prime powers (lesser = A064076).
 * https://oeis.org/A064077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Larger member of each pair of odd prime powers that
 *            differ by 2. Computation is performed by generating all odd
 *            prime powers up to a dynamically increasing limit, sorting them,
 *            and extracting the greater value of every consecutive pair with
 *            difference 2. The sequence is cached across calls
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t_cb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/cmp_seq_t_cb.h"

static seq_t *seq_vals = NULL;   /* cached sequence values   */
static size_t seq_len = 0;       /* number of values stored  */
static idx_t cur_limit = 0;      /* current sieve limit      */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n = 0 */

    while (seq_len < (size_t)n) {
        /* 1. Increase the sieve limit if necessary */
        if (cur_limit == 0) cur_limit = 2000000;
        else                cur_limit <<= 1;          /* double size */

        idx_t limit = cur_limit;

        /* 2. Sieve prime numbers up to 'limit' (only odd primes needed). */
        unsigned char *is_comp = (unsigned char *)calloc((size_t)limit + 1, 1);
        if (!is_comp) exit(1);
        is_comp[0] = is_comp[1] = 1;
        for (idx_t i = 2; i * i <= limit; ++i)
            if (!is_comp[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;

        /* Count odd primes ≥ 3 to allocate array. */
        idx_t primes_cnt = 0;
        for (idx_t i = 3; i <= limit; i += 2)
            if (!is_comp[i]) ++primes_cnt;

        idx_t *primes = NULL;
        if (primes_cnt) {
            primes = (idx_t *)malloc((size_t)primes_cnt * sizeof(idx_t));
            if (!primes) exit(1);
            idx_t idx = 0;
            for (idx_t i = 3; i <= limit; i += 2)
                if (!is_comp[i]) primes[idx++] = i;
        }

        free(is_comp);

        /* 3. Generate all odd prime powers ≤ limit. */
        size_t pow_cap = (size_t)primes_cnt * 2 + 16;
        seq_t *powers = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        if (!powers) exit(1);
        size_t pow_len = 0;

        for (idx_t pi = 0; pi < primes_cnt; ++pi) {
            idx_t p = primes[pi];
            seq_t cur = (seq_t)p;
            while (1) {
                if (pow_len == pow_cap) {
                    pow_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(powers, pow_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    powers = tmp;
                }
                powers[pow_len++] = cur;

                unsigned __int128 prod = (unsigned __int128)cur *
                                         (unsigned __int128)p;
                if (prod > (unsigned __int128)limit) break;
                cur = (seq_t)prod;
            }
        }

        free(primes);

        /* 4. Sort and deduplicate the power list. */
        qsort(powers, pow_len, sizeof(seq_t), cmp_seq_t_cb);
        size_t uniq_len = 0;
        if (pow_len) {
            uniq_len = 1;
            for (size_t i = 1; i < pow_len; ++i)
                if (powers[i] != powers[uniq_len - 1])
                    powers[uniq_len++] = powers[i];
        }

        /* 5. Extract the larger term of every twin pair. */
        size_t twin_cap = uniq_len + 1;
        seq_t *twins = (seq_t *)malloc(twin_cap * sizeof(seq_t));
        if (!twins) exit(1);
        size_t twin_len = 0;

        for (size_t i = 1; i < uniq_len; ++i) {
            if (powers[i] - powers[i - 1] == (seq_t)2) {
                if (twin_len == twin_cap) {
                    twin_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(twins, twin_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    twins = tmp;
                }
                twins[twin_len++] = powers[i];   /* larger element */
            }
        }

        free(powers);

        /* 6. Replace old cached sequence with the new one. */
        free(seq_vals);
        seq_vals = twins;
        seq_len  = twin_len;
    }

    return seq_vals[n - 1];
}
