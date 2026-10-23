/*
 * A171963  Number of partitions of the n-th semiprime into two semiprimes.
 * https://oeis.org/A171963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements a(n) = number of unordered partitions of the n-th semiprime into two semiprimes.
 * Uses a small linear sieve to generate semiprime numbers up to the n‑th one,
 * then counts partitions by checking each semiprime ≤ S/2.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t _semiprime_count = 0;
static size_t _semiprime_capacity = 0;
static seq_t *_semiprimes = NULL;
static bool *_is_semiprime = NULL;
static size_t _limit = 0;

/* Ensure that the sieve is large enough to contain at least `n` semiprimes. */
static void _ensure_precomputed(idx_t n)
{
    if (_semiprime_count >= (size_t)n) return;

    /* Increase limit until it contains enough semiprimes. */
    size_t new_limit = _limit ? _limit * 2 : 200000;
    while (true) {
        /* Rebuild sieve from scratch with new_limit. */
        uint32_t *spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
        int *cnt = (int *)calloc(new_limit + 1, sizeof(int));
        bool *is_sp = (bool *)calloc(new_limit + 1, sizeof(bool));
        size_t primes_cap = new_limit / 4 + 10;  /* rough upper bound on primes */
        uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        size_t primes_cnt = 0;

        /* Linear sieve for spf and prime list. */
        for (size_t i = 2; i <= new_limit; ++i) {
            if (!spf[i]) {
                spf[i] = (uint32_t)i;
                if (primes_cnt == primes_cap) {
                    primes_cap *= 2;
                    primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                }
                primes[primes_cnt++] = (uint32_t)i;
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint32_t p = primes[j];
                size_t ip = i * (size_t)p;
                if (p > spf[i] || ip > new_limit) break;
                spf[ip] = p;
            }
        }

        /* Count prime factors (with multiplicity). */
        cnt[1] = 0;
        for (size_t i = 2; i <= new_limit; ++i) {
            cnt[i] = cnt[i / spf[i]] + 1;
        }

        /* Collect semiprimes and mark them. */
        size_t sp_cnt = 0;
        size_t sp_cap = new_limit / 3 + 10;
        seq_t *sp_list = NULL;
        if (sp_cap > 0) {
            sp_list = (seq_t *)malloc(sp_cap * sizeof(seq_t));
        }

        for (size_t i = 2; i <= new_limit; ++i) {
            if (cnt[i] == 2) {
                if (sp_cnt == sp_cap) {
                    sp_cap = sp_cap ? sp_cap * 2 : 16;
                    sp_list = (seq_t *)realloc(sp_list, sp_cap * sizeof(seq_t));
                }
                sp_list[sp_cnt++] = (seq_t)i;
                is_sp[i] = true;
            }
        }

        /* Check if we have enough semiprimes. */
        if (sp_cnt >= (size_t)n) {
            /* Replace global data. */
            free(_semiprimes);
            free(_is_semiprime);
            _semiprimes = sp_list;
            _is_semiprime = is_sp;
            _semiprime_count = sp_cnt;
            _semiprime_capacity = sp_cap;
            _limit = new_limit;
            /* Clean up temporary arrays. */
            free(spf);
            free(cnt);
            free(primes);
            return;
        }

        /* Not enough: clean up and increase limit. */
        free(spf);
        free(cnt);
        free(is_sp);
        free(primes);
        free(sp_list);
        new_limit = new_limit * 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    _ensure_precomputed(n);
    seq_t target = _semiprimes[n - 1];
    seq_t count = 0;
    /* Count unordered partitions: iterate semiprimes <= target/2 */
    for (size_t i = 0; i < _semiprime_count; ++i) {
        seq_t a_val = _semiprimes[i];
        if (a_val > target / 2) break;
        size_t rem = (size_t)(target - a_val);
        if (rem <= _limit && _is_semiprime[rem]) {
            ++count;
        }
    }
    return count;
}
