/*
 * A049557  Primes p such that x^25 = 2 has a solution mod p.
 * https://oeis.org/A049557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which the congruence x^25 ≡ 2 (mod
 *            p) has a solution
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

static bool _cond(uint64_t p)
{
    if (p == 2) return true;               /* special case p = 2 */
    uint64_t m = p - 1;
    uint64_t g;
    if (m % 25 == 0)      g = 25;
    else if (m % 5 == 0)  g = 5;
    else                  g = 1;
    uint64_t exp = m / g;
    seq_t res = mod_pow((seq_t)2, exp, (seq_t)p);
    return res == (seq_t)1;
}

seq_t a(idx_t n)
{
    /* static state for memoised computation */
    static seq_t *selected = NULL;
    static size_t sel_cnt = 0, sel_cap = 0;
    static idx_t next_candidate = 2;
    static bool initialized = false;

    if (!initialized) {
        /* start with a modest capacity, will grow as needed */
        sel_cap = 256;
        selected = (seq_t*)malloc(sel_cap * sizeof(seq_t));
        initialized = true;
    }

    /* generate primes with the required property until we have n of them */
    while (sel_cnt < (size_t)n) {
        if (is_prime(next_candidate)) {
            if (_cond((uint64_t)next_candidate)) {
                if (sel_cnt >= sel_cap) {
                    sel_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(selected, sel_cap * sizeof(seq_t));
                    if (!tmp) abort();           /* allocation failure */
                    selected = tmp;
                }
                selected[sel_cnt++] = (seq_t)next_candidate;
            }
        }
        next_candidate++;
    }

    return selected[n - 1];
}
