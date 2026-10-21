/*
 * A056643  Numbers n such that n | 4^n + 3^n + 2^n + 1^n.
 * https://oeis.org/A056643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056643: Numbers n such that n | 4^n + 3^n + 2^n + 1^n.
   The function generates the sequence on demand by checking each integer
   from 1 upwards until the requested index is reached.
   It uses modular exponentiation with 128‑bit arithmetic to avoid
   overflow on the multiplications.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Modular exponentiation: computes (base^exp) mod mod, using 128‑bit
   intermediate products.  */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return result;
}

/* Test whether n divides 4^n + 3^n + 2^n + 1^n. */
static bool is_solution(uint64_t n)
{
    if (n == 0) return false;
    uint64_t mod = n;
    uint64_t sum = 1 % mod;              /* 1^n */
    sum += powmod_uint64(2, n, mod);     /* 2^n */
    sum %= mod;
    sum += powmod_uint64(3, n, mod);     /* 3^n */
    sum %= mod;
    sum += powmod_uint64(4, n, mod);     /* 4^n */
    sum %= mod;
    return sum == 0;
}

/* Public function: returns the nth term (n>=1) of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* stored sequence values */
    static size_t used = 0;        /* number of values stored so far */
    static size_t cap = 0;         /* capacity of the array */
    static uint64_t last_checked = 0; /* last integer examined */

    while (used < n) {
        ++last_checked;
        uint64_t candidate = last_checked;
        if (is_solution(candidate)) {
            if (used + 1 > cap) {
                size_t newcap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                seq = tmp;
                cap = newcap;
            }
            seq[used++] = (seq_t)candidate;
        }
    }
    return seq[n - 1];
}
