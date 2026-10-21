/*
 * A221838  Number of integer Heron triangles of height n.
 * https://oeis.org/A221838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A221837(n) + A046079(n) = A046079(n)^2 + A046079(n) where
 *            A046079(n) is the number of integer Heron triangles of height n
 *            that are isosceles. For an isosceles triangle the height equals
 *            n, the non‑equal sides are a and a, the base is 2x, and (a, x,
 *            n) is a Pythagorean triple. Thus A046079(...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* count trailing zero bits in an unsigned 64‑bit integer */
static inline unsigned int trailing_zeros_uint64(uint64_t v)
{
    unsigned int c = 0;
    while ((v & 1ULL) == 0ULL && v != 0ULL) {
        ++c;
        v >>= 1;
    }
    return c;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* offset begins at n = 1 */

    /* compute exponent k of 2 in n */
    uint64_t tmp = (uint64_t)n;
    unsigned int k = 0;
    while ((tmp & 1ULL) == 0ULL) {
        ++k;
        tmp >>= 1;
    }

    seq_t t = 0;                   /* number of suitable x (i.e., A046079(n)) */

    if ((n & 1ULL) == 0ULL) {      /* n is even */
        unsigned int twoK = k * 2; /* exponent of 2 in n^2 */

        for (idx_t d = 1; d < n; ++d) {
            /* check if d divides n^2 by testing (n % d)^2 % d == 0 */
            uint64_t r = (uint64_t)n % (uint64_t)d;
            if ((r * r) % (uint64_t)d != 0ULL) continue;

            if ((d & 1ULL) == 0ULL) {           /* d must be even */
                unsigned int tz = trailing_zeros_uint64((uint64_t)d);
                if (tz < twoK)                  /* ensure e is also even */
                    ++t;
            }
        }
    } else {                       /* n is odd */
        for (idx_t d = 1; d < n; ++d) {
            uint64_t r = (uint64_t)n % (uint64_t)d;
            if ((r * r) % (uint64_t)d == 0ULL) /* every divisor pair counts */
                ++t;
        }
    }

    /* a(n) = t^2 + t  (integer arithmetic in seq_t) */
    return t * t + t;
}
