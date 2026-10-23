/*
 * A104422  Numbers n such that n, p[n], p[n]+n, p[n]-n and p[n]*n all are
 *          2-less.
 * https://oeis.org/A104422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer n such that n, p[n], p[n]+n, p[n]-n and
 *            p[n]*n are all 2‑less (contain no decimal digit 2)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_2less
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_2less.h"

/* ---- prime generator ---- */
static uint64_t *prime_arr = NULL;
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

static void grow_prime_arr(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                 /* memory exhaustion */
    prime_arr = tmp;
    prime_cap = new_cap;
}

static void ensure_primes_up_to(size_t needed)
{
    while (prime_cnt < needed) {
        uint64_t cand = (prime_cnt ? prime_arr[prime_cnt - 1] + 1U : 2U);
        for (;;) {
            bool prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_arr[i];
                if (p * p > cand) break;
                if (cand % p == 0) { prime = false; break; }
            }
            if (prime) break;
            ++cand;
        }
        if (prime_cnt == prime_cap) grow_prime_arr();
        prime_arr[prime_cnt++] = cand;
    }
}

static seq_t get_prime(idx_t idx)
{
    ensure_primes_up_to((size_t)idx);
    return (seq_t)prime_arr[idx - 1];
}

/* ---- sequence function ---- */
seq_t a(idx_t n)
{
    idx_t  found = 0;
    seq_t  cand  = 0;

    while (1) {
        ++cand;

        if (!is_2less(cand)) continue;

        seq_t p = get_prime((idx_t)cand);
        if (!is_2less(p)) continue;
        if (!is_2less(p + cand)) continue;
        if (!is_2less(p - cand)) continue;
        if (!is_2less(p * cand)) continue;

        ++found;
        if (found == n) return cand;
    }
}
