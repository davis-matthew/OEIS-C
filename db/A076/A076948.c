/*
 * A076948  Smallest k such that nk-1 is a square, or 0 if no such number
 *          exists.
 * https://oeis.org/A076948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k>0 with n*k-1 a perfect square; 0 if none
 * Range:     u8 n <= 1092, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches for the smallest m with m²≡-1 (mod n).  If such a
 * m exists, then k = (m²+1)/n.  The search stops at m = n-1; for n=1 we
 * return 1 immediately.  All arithmetic that can overflow 64‑bit is done
 * in 128‑bit to guarantee correctness for n up to at least 100 000. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n=0 – return 0 as a safe guard. */
    if (n == 0) return 0;

    /*  n = 1  is a trivial case: 1*1-1 = 0². */
    if (n == 1) return 1;

    uint64_t mod   = (uint64_t)n;
    uint64_t target = mod - 1;               /* -1 modulo n */

    for (idx_t m = 1; m < n; ++m) {
        /* Compute m² modulo n using 128‑bit intermediate to avoid overflow. */
        __uint128_t prod = (__uint128_t)m * m;
        uint64_t rem = (uint64_t)(prod % mod);

        if (rem == target) {
            /* k = (m² + 1) / n  (exact division). */
            __uint128_t numer = prod + 1;
            seq_t k = (seq_t)(numer / mod);
            return k;
        }
    }
    /* No quadratic residue -1 exists modulo n */
    return 0;
}
