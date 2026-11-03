/*
 * A224469  Primes of the form 8^n - n.
 * https://oeis.org/A224469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A224469
 * The sequence lists primes of the form 8^k - k in order of increasing k,
 * starting with k = 1.
 * The function a(idx_t n) returns the n-th prime in this list.
 */

#include <stdlib.h>
#include <stdbool.h>
/* We assume the helper functions are available in the build environment. */
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

seq_t a(idx_t n) {
    /* static state to enable memoisation of previously computed terms */
    static idx_t count = 0;          /* number of primes already found */
    static idx_t k_next = 1;         /* next k to test */
    static seq_t *primes = NULL;     /* dynamic array of found primes */
    static size_t cap = 0;           /* capacity of the primes array */

    /* keep computing until we have at least n primes */
    while (count < n) {
        /* candidate prime: 8^k_next - k_next */
        seq_t candidate = ipow_i32((seq_t)8, (int)k_next) - (seq_t)k_next;

        /* test primality */
        if (is_prime_big(candidate)) {
            /* resize array if necessary */
            if (count >= cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) return (seq_t)0; /* allocation failure – return zero */
                primes = tmp;
                cap = new_cap;
            }
            /* store the prime */
            primes[count++] = candidate;
        }
        k_next++;  /* advance to the next k */
    }

    /* return the n-th prime (1‑based index) */
    return primes[n - 1];
}
