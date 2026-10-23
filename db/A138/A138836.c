/*
 * A138836  Non-Mersenne numbers A001348.
 * https://oeis.org/A138836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer that is NOT of the form 2^p‑1 with p prime */

#include <stdbool.h>

/* Check primality for small exponents (p ≤ 64, since 2^p‑1 grows very fast). */
static bool is_prime_small(unsigned long long x) {
    if (x < 2) return false;
    unsigned long long m = 2;
    while (m * m <= x) {
        if (x % m == 0) return false;
        ++m;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used for n ≥ 1, but harmless */
    idx_t i   = 1;                  /* candidate integer */
    idx_t cnt = 0;                  /* count of accepted numbers */

    for (;;) {
        bool excl = false;
        idx_t ip1 = i + 1;

        /* Test if i + 1 is a power of two (i.e., i is of the form 2^p‑1). */
        if ((ip1 & i) == 0) {
            unsigned exp = 0;
            idx_t t = ip1;
            while ((t & 1) == 0) {   /* count trailing zeros → log₂(ip1) */
                t >>= 1;
                ++exp;
            }
            if (is_prime_small((unsigned long long)exp))
                excl = true;        /* 2^p‑1 with p prime → skip */
        }

        if (!excl) {                /* accept the number */
            ++cnt;
            if (cnt == n)
                return (seq_t)i;    /* cast to the sequence type */
        }
        ++i;                        /* next candidate */
    }
}
