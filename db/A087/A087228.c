/*
 * A087228  a(n) is the smallest number k such that the LCM of the terms of
 *          the Collatz trajectory of k has n distinct prime factors.
 * https://oeis.org/A087228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   add_unique_uint64, factorize, next_collatz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087228:
   a(n) is the smallest integer k such that the LCM of the terms
   of the Collatz trajectory of k has exactly n distinct prime factors. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize.h"
#include "../utils/add_unique_uint64.h"
#include "../utils/next_collatz.h"

/* Factor a number and add its distinct prime factors to set. */
static void factor_and_record(uint64_t x, uint64_t *set, int *sz) {
    if (x <= 1) return;
    uint64_t primes[64];
    int exps[64];
    int cnt = factorize(x, primes, exps);
    for (int i = 0; i < cnt; ++i)
        add_unique_uint64(primes[i], set, sz);
}

/* Count distinct primes appearing in the LCM of the Collatz trajectory of k. */
static int collatz_lcm_prime_count(uint64_t k) {
    uint64_t primes_set[128];
    int sz = 0;
    uint64_t v = k;
    while (v != 1) {
        factor_and_record(v, primes_set, &sz);
        v = next_collatz(v);
    }
    /* Term 1 contributes no primes. */
    return sz;
}

/* Main function required by the harness. */
seq_t a(idx_t n) {
    /* Memoised answers: ans[i] = smallest k having i distinct primes. */
    static seq_t *ans = NULL;
    static idx_t ans_cap = 0;
    static uint64_t cur_k = 1;  /* last examined k (starts with 1) */

    /* Ensure storage is large enough for n */
    if (n >= ans_cap) {
        idx_t new_cap = ans_cap == 0 ? 1 : ans_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(ans, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        if (new_cap > ans_cap) {
            for (idx_t i = ans_cap; i < new_cap; ++i) tmp[i] = 0;
        }
        ans = tmp;
        ans_cap = new_cap;
    }

    /* Compute next k values until we have an answer for the requested n. */
    while (ans[n] == 0) {
        ++cur_k;
        int cnt = collatz_lcm_prime_count(cur_k);
        if (cnt > 0) {
            /* Resize the array if the count exceeds current capacity. */
            if ((idx_t)cnt >= ans_cap) {
                idx_t new_cap = ans_cap == 0 ? 1 : ans_cap;
                while (new_cap <= (idx_t)cnt) new_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(ans, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                if (new_cap > ans_cap) {
                    for (idx_t i = ans_cap; i < new_cap; ++i) tmp[i] = 0;
                }
                ans = tmp;
                ans_cap = new_cap;
            }
            if (ans[cnt] == 0)
                ans[cnt] = (seq_t)cur_k;
        }
    }
    return ans[n];
}
