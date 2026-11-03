/*
 * A019385  Primes with primitive root 61.
 * https://oeis.org/A019385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 984 (2026-11-01)
 * Helpers:   factor_u64, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = the nth prime p (p ≠ 61) such that 61 is a primitive root modulo p */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *found = NULL;
    static size_t found_len = 0;
    static size_t found_cap = 0;
    static uint64_t current = 2;      /* search starts at 2 */

    while (found_len < (size_t)n) {
        /* get next prime >= current */
        uint64_t p = next_prime(current - 1);
        current = p + 1;

        if (p == 61) continue;      /* 61 is not coprime with itself */

        uint64_t pf[64], ex[64];
        size_t cnt = 0;
        factor_u64(p - 1, pf, ex, &cnt);

        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            uint64_t q = pf[i];
            seq_t r = powmod((seq_t)61, (p - 1) / q, (seq_t)p);
            if (r == 1) ok = false;
        }

        if (ok) {
            if (found_len == found_cap) {
                size_t new_cap = found_cap ? found_cap * 2 : 8;
                seq_t *tmp = seq_realloc(found, found_cap, new_cap);
                if (!tmp) return 0;   /* allocation failure, unlikely */
                found = tmp;
                found_cap = new_cap;
            }
            found[found_len++] = (seq_t)p;
        }
    }

    return found[n - 1];
}
