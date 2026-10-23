/*
 * A112859  Primes such that the sum of the predecessor and successor primes
 *          is divisible by 29.
 * https://oeis.org/A112859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(i) is in this sequence iff prime(i-1)+prime(i+1) =
 *            0 mod 29
 * Range:     u8 n <= 1, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/simple_sieve.h"

/* static data for the sequence */
static uint32_t *prime_arr = NULL;
static size_t      prime_cnt = 0;
static seq_t      *seq_vals  = NULL;
static size_t      seq_cnt   = 0;
static bool        seq_ready = false;

/* build the sequence once */
static void build_sequence(void)
{
    if (seq_ready) return;

    /* an upper bound large enough to contain about 44 000 terms */
    const uint32_t LIMIT = 20000000u;  /* 20 million */

    /* generate all primes up to LIMIT */
    simple_sieve(LIMIT, &prime_arr, &prime_cnt);

    /* reserve space for the result array */
    size_t alloc_cap = 65536;          /* initial capacity */
    seq_vals = (seq_t *)malloc(alloc_cap * sizeof(seq_t));
    seq_cnt = 0;

    /* scan primes and collect those satisfying the condition */
    for (size_t j = 1; j + 1 < prime_cnt; ++j) {
        uint64_t a = (uint64_t)prime_arr[j - 1];
        uint64_t b = (uint64_t)prime_arr[j + 1];
        if (((a + b) % 29) == 0) {
            if (seq_cnt == alloc_cap) {
                alloc_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, alloc_cap * sizeof(seq_t));
            }
            seq_vals[seq_cnt++] = (seq_t)prime_arr[j];
        }
    }

    seq_ready = true;
}

/* OEIS routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */
    build_sequence();
    size_t idx = (size_t)(n - 1ull);
    return (idx < seq_cnt) ? seq_vals[idx] : 0;
}
