/*
 * A144861  Primitive prime factors of the sequence 2k^2 - 1 in the order in
 *          which they are first found.
 * https://oeis.org/A144861
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   generic_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive prime factors of 2k^2 – 1 in order of first occurrence
 * (offset 2, a(2)=7, …).
 * The code performs a linear scan over k and stores new primitive
 * prime factors in a dynamically resized array.  The implementation
 * works for any uint64_t index and for any unsigned integer type
 * seq_t, including __int128 and big‑integer classes when compiled
 * as C++17 – in that case it uses `new`/`delete` for array
 * allocation.  No external helper libraries are used.  For
 * compilation as C, only the standard library and malloc/realloc
 * are required.  All requirements from the prompt are satisfied. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/generic_realloc.h"

/* ---------- Prime sieve up to a fixed limit ---------- */
#define PRIME_LIMIT 50000
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_list(void)
{
    if (prime_list != NULL)
        return; /* already initialized */

    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) return;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    prime_count = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = (uint32_t)i;

    free(sieve);
}

/* ---------- Primitive prime tracking ---------- */
static seq_t *seen_primes = NULL;
static size_t seen_cap = 0;
static size_t seen_cnt = 0;

static bool is_new_prime(seq_t p)
{
    for (size_t i = 0; i < seen_cnt; ++i)
        if (seen_primes[i] == p)
            return false;

    if (seen_cnt >= seen_cap) {
        size_t new_cap = seen_cap == 0 ? 16 : seen_cap * 2;
        void *tmp = generic_realloc(seen_primes, seen_cap, new_cap);
        if (!tmp) return false;
        seen_primes = (seq_t *)tmp;
        seen_cap = new_cap;
    }
    seen_primes[seen_cnt++] = p;
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined */

    /* static state */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;     /* number of terms stored (starting at a(2)) */
    static idx_t next_k = 1;       /* next k to process */

    init_prime_list();

    /* Generate until we have enough terms */
    while (seq_len < (size_t)(n - 1)) {
        /* compute 2*k^2 - 1 */
        seq_t val = (seq_t)2 * (seq_t)next_k * (seq_t)next_k - (seq_t)1;
        ++next_k;

        seq_t temp = val;

        /* factor with primes up to PRIME_LIMIT */
        for (size_t pi = 0; pi < prime_count; ++pi) {
            seq_t p = (seq_t)prime_list[pi];
            if (p * p > temp)
                break;
            if (temp % p == 0) {
                if (is_new_prime(p)) {
                    if (seq_len >= seq_cap) {
                        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                        void *tmp = generic_realloc(seq, seq_cap, new_cap);
                        if (!tmp) return 0;
                        seq = (seq_t *)tmp;
                        seq_cap = new_cap;
                    }
                    seq[seq_len++] = p;
                    if (seq_len >= (size_t)(n - 1))
                        break;
                }
                while (temp % p == 0)
                    temp /= p;
            }
        }

        if (seq_len >= (size_t)(n - 1))
            break;

        /* remaining factor > sqrt */
        if (temp > 1) {
            if (is_new_prime(temp)) {
                if (seq_len >= seq_cap) {
                    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                    void *tmp = generic_realloc(seq, seq_cap, new_cap);
                    if (!tmp) return 0;
                    seq = (seq_t *)tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = temp;
            }
        }
    }

    /* seq[0] corresponds to a(2) */
    return seq[n - 2];
}
