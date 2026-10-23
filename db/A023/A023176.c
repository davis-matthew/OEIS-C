/*
 * A023176  Numbers k such that Fibonacci(k) == 5 (mod k).
 * https://oeis.org/A023176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 841, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reduced to search: a(n) is the n-th integer k with Fibonacci(k) ≡ 5 (mod k) */

#include <stdint.h>
#include <stdlib.h>

/* Compute F(n) mod mod using fast doubling (recursive). */
static uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    if (mod == 1) return 0;
    if (n == 0) return 0 % mod;
    /* recursive helper */
    struct pair { uint64_t f; uint64_t f_next; };
    static struct pair fn[64] = {0};
    /* Use recursion depth <= 64 */
    /* helper function defined inline using loop to avoid recursion overhead */
    /* We'll implement iterative stack */
    uint64_t a0 = 0, a1 = 1 % mod;    /* F(0), F(1) */
    for (int i = 63; i >= 0; --i) {
        /* Doubling formulas */
        uint64_t two_b = (uint64_t)(((__uint128_t)a1 * 2) % mod);
        uint64_t t1 = (uint64_t)(((__uint128_t)a0 * ((two_b + mod - a0) % mod)) % mod); /* F(2k) */
        uint64_t t2 = (uint64_t)(((__uint128_t)a0 * a0 + (__uint128_t)a1 * a1) % mod);     /* F(2k+1) */
        if ((n >> i) & 1) {
            a0 = t2;
            a1 = (t1 + t2) % mod;
        } else {
            a0 = t1;
            a1 = t2;
        }
    }
    return a0;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t count = 0;
    static idx_t capacity = 0;
    static uint64_t last_k = 0;

    while (count < n) {
        ++last_k;
        uint64_t mod = last_k;
        uint64_t f_mod = fib_mod_uint64(last_k, mod);
        if (f_mod == 5 % mod) {
            if (count + 1 > capacity) {
                size_t new_cap = capacity == 0 ? 8 : capacity * 2;
                seq_t *new_ptr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_ptr) abort();
                terms = new_ptr;
                capacity = new_cap;
            }
            terms[count++] = (seq_t)last_k;
        }
    }
    return terms[n - 1];
}
