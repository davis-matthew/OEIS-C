/*
 * A082058  a(n) is the smallest k such that prime(k) contains the digits of n
 *          as a substring.
 * https://oeis.org/A082058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that prime(k) contains the decimal
 *            representation of n as a substring. This implementation
 *            (trial‑division prime generator + string matching)
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ---------- small utility: primality test for uint64_t ---------- */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v == 2 || v == 3) return true;
    if ((v & 1) == 0) return false;      /* even numbers > 2 are composite */
    for (uint64_t d = 3; d * d <= v; d += 2) {
        if (v % d == 0) return false;
    }
    return true;
}

/* ---------- main function ------------------------------------- */
seq_t a(idx_t n)
{
    /* convert n to its decimal string; 32 bytes enough for 64‑bit numbers */
    char pat[32];
    int pat_len = snprintf(pat, sizeof pat, "%llu", (unsigned long long)n);
    pat[pat_len] = '\0';

    /* walk through primes starting from 2 */
    uint64_t prime = 2;
    idx_t prime_idx = 1;      /* 1‑based index of current prime */

    while (1) {
        /* test whether prime string contains the pattern */
        char buf[32];
        int len = snprintf(buf, sizeof buf, "%llu", prime);
        buf[len] = '\0';
        if (strstr(buf, pat)) {
            return (seq_t)prime_idx;
        }

        /* find next prime */
        do {
            prime++;
            if (prime == 2) continue;        /* keep odd after 2 */
            if ((prime & 1) == 0) prime++;   /* skip even numbers */
        } while (!is_prime_uint64(prime));
        prime_idx++;
    }
}
