/*
 * A097751  Least integer with same "mod 4 prime signature" as n.
 * https://oeis.org/A097751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal integer with the same “mod 4 prime signature” as
 *            n. The signature consists of a₀=exponent of 2, followed by
 *            exponents of primes ≡3 (mod 4) (sorted descending), then
 *            exponents of primes ≡1 (mod 4) (sorted descending). The minimal
 *            integer is obtained by replacing each 3 (mod 4) prime...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   cmp_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp_desc.h"

/* smallest primes ≡3 (mod 4) and ≡1 (mod 4) – enough for any n≤2⁶⁴ */
static const unsigned primes3[20] = {
    3, 7, 11, 19, 23, 31, 43, 47, 59, 67,
    71, 79, 83, 103, 107, 127, 131, 139, 151, 163
};
static const unsigned primes1[20] = {
    5, 13, 17, 29, 37, 41, 53, 61, 73, 89,
    97, 101, 109, 113, 137, 149, 157, 173, 181, 193
};

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1)=1 */

    idx_t temp = n;
    unsigned a0 = 0;

    /* exponent of 2 */
    while ((temp & 1ULL) == 0) {
        ++a0;
        temp >>= 1;
    }

    /* arrays for exponents of primes 3‑mod‑4 and 1‑mod‑4 */
    unsigned ex3[20] = {0};
    unsigned ex1[20] = {0};
    int cnt3 = 0, cnt1 = 0;

    idx_t p = 3;
    while (p * p <= temp) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            if (p % 4 == 3) {
                ex3[cnt3++] = e;
            } else {
                /* p % 4 == 1 for odd primes at this point */
                ex1[cnt1++] = e;
            }
        }
        p += 2;
    }
    if (temp > 1) {                  /* remaining prime factor */
        if (temp % 4 == 3) {
            ex3[cnt3++] = 1;
        } else {
            ex1[cnt1++] = 1;
        }
    }

    /* sort exponents descending to match signature ordering */
    if (cnt3 > 1) qsort(ex3, cnt3, sizeof(unsigned), cmp_desc);
    if (cnt1 > 1) qsort(ex1, cnt1, sizeof(unsigned), cmp_desc);

    /* build minimal integer */
    seq_t result = 1;

    /* 2^a0 */
    for (unsigned i = 0; i < a0; ++i) {
        result *= 2;
    }

    /* primes ≡3 (mod 4) */
    for (int i = 0; i < cnt3; ++i) {
        seq_t base = primes3[i];
        for (unsigned e = 0; e < ex3[i]; ++e) {
            result *= base;
        }
    }

    /* primes ≡1 (mod 4) */
    for (int i = 0; i < cnt1; ++i) {
        seq_t base = primes1[i];
        for (unsigned e = 0; e < ex1[i]; ++e) {
            result *= base;
        }
    }

    return result;
}
