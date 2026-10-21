/*
 * A200517  Least m>0 such that n = y^2 - 7^x (mod m) has no solution, or 0 if
 *          no such m exists.
 * https://oeis.org/A200517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if there exist integers y,x with y*y-7^x=n; otherwise
 *            a(n) = the least m>0 such that the congruence y*y-7^x ≡ n (mod
 *            m) has no solution
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Integer square root for 64‑bit unsigned values. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* highest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Upper bound for search of m. 65536 is ample for the ranges expected. */
#define MAX_M 65536
static uint8_t qr[MAX_M + 1];           /* temporary QR sets */

/* --------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t un = (uint64_t)n;          /* promote to 64‑bit for safety */

    /* 1. Check for exact representation y^2 - 7^x = n */
    {
        uint64_t pow = 1;
        while (true) {
            uint64_t target = un + pow;                /* y*y = target */
            uint64_t rt = isqrt_u64(target);
            if (rt * rt == target)                    /* exact square found */
                return (seq_t)0;

            if (pow > ULLONG_MAX / 7) break;          /* prevent overflow   */
            pow *= 7;                                 /* next power of 7    */
        }
    }

    /* 2. Search for the smallest modulus m that blocks all solutions. */
    for (uint64_t m = 1; m <= MAX_M; ++m) {
        /* Build the set of quadratic residues modulo m. */
        for (uint64_t i = 0; i < m; ++i) qr[i] = 0;
        for (uint64_t y = 0; y < m; ++y) {
            uint64_t sq = (y * y) % m;
            qr[sq] = 1;
        }

        /* Iterate over the powers of 7 modulo m. */
        uint64_t val = 1 % m;                      /* 7^0 mod m */
        bool found = false;
        for (uint64_t t = 0; t < m; ++t) {
            uint64_t target = (un + val) % m;      /* need y^2 ≡ target (mod m) */
            if (qr[target]) {                      /* solution exists */
                found = true;
                break;
            }
            val = (val * 7) % m;                   /* next power of 7    */
        }

        if (!found)  /* no solution modulo m */
            return (seq_t)m;
    }

    /* If we exceeded the search bound, assume no such modulus exists. */
    return (seq_t)0;
}
