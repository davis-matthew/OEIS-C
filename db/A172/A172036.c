/*
 * A172036  Prime partial sums of safe primes A005385.
 * https://oeis.org/A172036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th prime partial sum of safe primes (partial sums that
 *            are themselves prime)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n) {
    /* static state to allow out-of-order calls */
    static idx_t curr = 3;          /* next odd number to test for primality      */
    static seq_t running_sum = 0;   /* running sum of safe primes                */
    static seq_t *partial_sums = NULL;
    static size_t ps_len = 0, ps_cap = 0;

    if (n == 0) return 0; /* offset irrelevant, sequence is 1‑based */

    /* generate terms until we have at least n of them */
    while (ps_len < (size_t)n) {
        /* find next prime */
        while (!is_prime(curr)) {
            curr += 2;                /* skip even numbers */
        }

        /* check if the prime is a safe prime (p-1)/2 is also prime */
        if (is_prime((curr - 1) / 2)) {
            running_sum += (seq_t)curr;  /* add to running sum */

            /* if the sum is prime, store it */
            if (is_prime_big(running_sum)) {
                if (ps_len == ps_cap) {
                    size_t new_cap = ps_cap ? ps_cap * 2 : 4;
                    seq_t *tmp = seq_realloc(partial_sums, ps_cap, new_cap);
                    if (!tmp) return 0; /* allocation failure */
                    partial_sums = tmp;
                    ps_cap = new_cap;
                }
                partial_sums[ps_len++] = running_sum;
            }
        }

        curr += 2;  /* next odd candidate */
    }

    return partial_sums[n - 1];
}
