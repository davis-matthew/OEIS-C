/*
 * A176072  Primes of the form 3^k + 2^k + k + 1.
 * https://oeis.org/A176072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = the nth prime of the form 3^k + 2^k + k + 1 (OEIS
 *            A176072)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, but safe for caller */

    /* Static state to cache computed terms. */
    static size_t capacity = 0;          /* allocated slots */
    static seq_t *values   = NULL;       /* array of found primes   */
    static idx_t  count    = 0;          /* primes stored so far   */
    static idx_t  next_k   = 1;          /* next exponent to test  */

    /* Initial allocation. */
    if (values == NULL) {
        capacity = 16;
        values   = mem_new(seq_t, capacity);
    }

    /* Find primes until we have at least n of them. */
    while (count < n) {
        /* Compute 3^k + 2^k + k + 1 using provided big power helper. */
        seq_t val = ipow((seq_t)3, (idx_t)next_k);
        val += ipow((seq_t)2, (idx_t)next_k);
        val += (seq_t)next_k;   /* add k   */
        val += (seq_t)1;        /* add 1   */

        /* Test primality. */
        if (is_prime_big(val)) {
            /* Grow array if necessary. */
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = mem_resize(seq_t, values, new_cap);
                if (tmp == NULL) {   /* allocation failure – abort */
                    mem_free(values);
                    return 0;
                }
                values = tmp;
                capacity = new_cap;
            }
            values[count] = val;
            count++;
        }
        next_k++;
    }

    return values[n - 1];
}
