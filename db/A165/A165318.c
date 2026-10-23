/*
 * A165318  Primes p where the number of divisors of p-1 is a power of 2.
 * https://oeis.org/A165318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3307, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165318: primes p where the number of divisors of p-1 is a power of 2 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static seq_t *term_cache = NULL;
static size_t term_cap  = 0;
static size_t term_len  = 0;
static uint64_t last_checked = 1;   /* last number examined */

static void add_term(seq_t val)
{
    if (term_cap == 0) {
        term_cap = 16;
        term_cache = (seq_t *)malloc(term_cap * sizeof(seq_t));
    } else if (term_len == term_cap) {
        term_cap <<= 1;
        term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
    }
    term_cache[term_len++] = val;
}

static bool is_power_of_two_uint64(uint64_t x)
{
    return x && ((x & (x - 1)) == 0);
}

static void build_until(idx_t target)
{
    if (target < term_len)
        return;
    /* target is 0‑based index of last required element */
    while (term_len <= target) {
        uint64_t cand = last_checked + 1;
        if (cand <= 2)
            cand = 2;
        else if ((cand & 1) == 0)
            cand++;             /* skip even numbers >2 */

        for (;;) {
            if (is_prime((idx_t)cand)) {
                bool ok;
                if (cand == 2) {
                    ok = true;
                } else {
                    uint64_t m = cand - 1;
                    uint64_t primes[64], exps[64];
                    size_t cnt = 0;
                    factor_u64(m, primes, exps, &cnt);
                    ok = true;
                    for (size_t i = 0; i < cnt; ++i) {
                        uint64_t deg = exps[i] + 1;
                        if (!is_power_of_two_uint64(deg)) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) {
                    add_term((seq_t)cand);
                    last_checked = cand;
                    break;
                }
            }
            cand += 2;           /* next odd candidate */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 as placeholder */
    build_until(n - 1);
    return term_cache[n - 1];
}
