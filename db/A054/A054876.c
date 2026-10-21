/*
 * A054876  Number of pairwise incongruent triangles with integer sides and
 *          positive integer area and second longest side of length n.
 * https://oeis.org/A054876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct integer‑sided triangles with middle
 *            side length n, integer area, and positive area
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root of a 64‑bit unsigned integer (floor). */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;          /* largest power of 4 ≤ x  */
    while (bit) {
        uint64_t next = r + bit;
        if (next <= x) {
            x -= next;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Count triangles with second longest side equal to n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    uint64_t nn = (uint64_t)n;
    uint64_t count = 0;

    for (uint64_t a = 1; a <= nn; ++a) {     /* a is the smallest side */
        /* c ranges from n to a + n - 1 (triangle inequality) */
        uint64_t c_start = nn;
        uint64_t c_end   = a + nn - 1;
        for (uint64_t c = c_start; c <= c_end; ++c) {
            uint64_t sum = a + nn + c;       /* a + b + c  (b = n) */
            if (sum & 1ULL) continue;        /* perimeter must be even */

            /* Compute the four factors in the Heron product */
            uint64_t p   = sum >> 1;          /* semiperimeter  */
            uint64_t v   = (nn + c - a) >> 1; /* p - a          */
            uint64_t w   = (a + c - nn) >> 1; /* p - n          */
            uint64_t t   = (a + nn - c) >> 1; /* p - c          */

            /* area² = p * (p-a) * (p-n) * (p-c) */
            uint64_t prod = p * v;
            prod *= w;
            prod *= t;

            /* Check if prod is a perfect square */
            uint64_t r = isqrt64(prod);
            if (r * r == prod) ++count;
        }
    }

    return (seq_t)count;
}
