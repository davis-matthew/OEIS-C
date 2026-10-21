/*
 * A075016  Smallest k such that the concatenation k, k-1,k-2 is divisible by
 *          n; or 0 if no such number exists.
 * https://oeis.org/A075016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that the decimal concatenation of k, k-1
 *            and k-2 is divisible by n; 0 if no such k exists
 * Range:     u8 n <= 181, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10mod.h"

/* number of decimal digits of a non‑negative integer (0 has 1 digit) */
static unsigned int digits_uint64(uint64_t x)
{
    if (x == 0) return 1;
    unsigned int d = 0;
    while (x) {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 2  (by definition) */
        return 2;

    idx_t mod = n;       /* modulus */
    for (idx_t k = 2; k; ++k) {      /* start at k = 2, 0 terminates on overflow */
        uint64_t km1 = (uint64_t)(k - 1);   /* k-1, k-2 fit in 64 bits for the intended range */
        uint64_t km2 = (uint64_t)(k - 2);

        unsigned int d1 = digits_uint64(km1);   /* digits of k-1 */
        unsigned int d2 = digits_uint64(km2);   /* digits of k-2 */

        seq_t r = ((seq_t)k) % (seq_t)mod;
        r = (r * pow10mod(d1, mod) + ((seq_t)km1) % (seq_t)mod) % (seq_t)mod;
        r = (r * pow10mod(d2, mod) + ((seq_t)km2) % (seq_t)mod) % (seq_t)mod;

        if (r == 0)
            return k;
        if (k == (idx_t)-1)            /* overflow guard */
            break;
    }
    return 0;       /* no such k found within the representable range */
}
