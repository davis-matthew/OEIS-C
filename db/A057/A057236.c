/*
 * A057236  Numbers n such that n | 5^n + 4^n + 3^n.
 * https://oeis.org/A057236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   mul_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057236: Numbers n such that n | 5^n + 4^n + 3^n. 
 * Computed by checking each integer n. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_mod_u64.h"

/* 64‑bit modular multiplication using 128‑bit intermediate. */

/* 64‑bit modular exponentiation. */
static inline uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1u)
            result = mul_mod_u64(result, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

/* Return the n-th term of A057236. */
seq_t a(idx_t n)
{
    /* Cached sequence values. */
    static seq_t *seq = NULL;
    static size_t  seq_len = 0, seq_cap = 0;
    /* Last integer examined. */
    static idx_t last = 0;

    /* Allocate or enlarge cache on first use. */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        ++last;                    /* candidate integer m */
        uint64_t m = (uint64_t)last;

        /* Compute (5^m + 4^m + 3^m) mod m. */
        uint64_t s = pow_mod_u64(5ull, m, m);
        s += pow_mod_u64(4ull, m, m);
        if (s >= m) s -= m;
        s += pow_mod_u64(3ull, m, m);
        s %= m;

        /* If m divides the sum, store it. */
        if (s == 0u) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)m;
        }
    }

    return seq[n - 1];
}
