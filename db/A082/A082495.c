/*
 * A082495  a(n) = (2^n - 1) mod n.
 * https://oeis.org/A082495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1) mod n
 * Range:     u8 n <= 294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n ≥ 1. Handle n == 0 by returning 0 (unused). */
    if (n == 0) return (seq_t)0;

    /* Compute 2^n mod n using 128‑bit arithmetic for safe overflow. */
    uint64_t mod = (uint64_t)n;
    unsigned __int128 res = 1;
    unsigned __int128 base = 2 % mod;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    seq_t pow = (seq_t)res;          /* 2^n mod n  (0 ≤ pow < n) */
    seq_t mod_seq = (seq_t)mod;

    /* a(n) = (pow - 1) mod n.
       If pow == 0 we need to return n - 1; otherwise pow-1 is already correct. */
    if (pow == 0) return (seq_t)(mod - 1);
    return pow - 1;
}
