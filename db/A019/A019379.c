/*
 * A019379  Primes with primitive root 55.
 * https://oeis.org/A019379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p for which 55 is a primitive root modulo p
   Computed by incremental search and primality testing.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t x) */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n,uint64_t primes[],uint64_t exps[],size_t *cnt) */
#include "../utils/powmod64.h"     /* uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod) */

static seq_t *special = NULL;
static size_t special_cnt = 0;
static size_t special_cap = 0;
static uint64_t last_checked = 1;

/* Ensure the special array has room for at least 'need' elements. */
static void grow_special(size_t need)
{
    if (special_cap >= need)
        return;
    size_t new_cap = special_cap ? special_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    special = tmp;
    special_cap = new_cap;
}

/* Test whether 55 is a primitive root modulo prime p. */
static bool is_primitive_root_55(uint64_t p)
{
    /* 55 must be coprime to p */
    if (p == 5 || p == 11)   /* 55 is divisible by 5 or 11 */
        return false;
    if (p == 2)
        return true;         /* 55 ≡ 1 (mod 2), order = 1 = p-1 */

    uint64_t rem = p - 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(rem, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        uint64_t exp = rem / q;
        uint64_t r = powmod64(55ULL, exp, p);
        if (r == 1)
            return false;     /* not a primitive root */
    }
    return true;
}

/* OEIS sequence A019379: primes with primitive root 55. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0, but safe */

    if (special_cnt < (size_t)n) {
        grow_special(n);
        while (special_cnt < (size_t)n) {
            ++last_checked;
            uint64_t p = last_checked;

            /* skip even numbers > 2 */
            if (!((p == 2) || (p & 1)))
                continue;
            if (!is_prime(p))
                continue;
            if (is_primitive_root_55(p)) {
                if (special_cnt >= special_cap)
                    grow_special(special_cnt + 1);
                special[special_cnt++] = (seq_t)p;
            }
        }
    }
    return special[n - 1];
}
