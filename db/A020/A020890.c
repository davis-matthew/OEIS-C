/*
 * A020890  Ordered set of (b + c - a)/2 as (a,b,c) runs through all primitive
 *          Pythagorean triples with a < b < c.
 * https://oeis.org/A020890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A020890(n), the ordered set of (b + c - a)/2 for
 *            primitive Pythagorean triples with a < b < c
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates all primitive triples up to a fixed m‑max, computes the values, sorts them, and returns the n‑th element. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Euclidean gcd for uint64_t */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Static storage for computed values */
static seq_t *values = NULL;        /* array of computed terms */
static size_t   values_len = 0;      /* number of terms stored */

/* Generate all values up to a fixed maximum of m */
static void generate_all(void)
{
    if (values_len > 0) return;            /* already generated */

    /* Set an upper bound for m. 3000 gives ~1.4 million values, enough for n up to many thousands. */
    const uint64_t MMAX = 3000;

    /* Reserve a large enough buffer to avoid many reallocs. */
    size_t reserve = (size_t)MMAX * (MMAX - 1) / 4;   /* rough estimate of valid pairs */
    values = (seq_t *)malloc(reserve * sizeof(seq_t));
    if (!values) return; /* out of memory */

    for (uint64_t m = 2; m <= MMAX; ++m)
    {
        for (uint64_t k = 1; k < m; ++k)
        {
            /* require opposite parity */
            if (((m ^ k) & 1u) == 0) continue;
            /* require coprime */
            if (gcd_u64(m, k) != 1u) continue;

            /* Determine whether a<b or a>b and compute value accordingly.
               a = |m^2 - k^2|, b = 2mk, c = m^2 + k^2
               When (m-k)^2 < 2*k^2, a = m^2 - k^2 < b and we use k*(m+k).
               Otherwise a = 2mk < b and we use m*(m-k). */
            __uint128_t diff = (__uint128_t)(m - k);
            __uint128_t diffSq = diff * diff;
            __uint128_t twoKsq = (__uint128_t)2 * k * k;

            seq_t val;
            if (diffSq < twoKsq)
            {
                /* val = k * (m + k) */
                __uint128_t prod = (__uint128_t)k * (m + k);
                val = (seq_t)prod;
            }
            else
            {
                /* val = m * (m - k) */
                __uint128_t prod = (__uint128_t)m * (m - k);
                val = (seq_t)prod;
            }

            /* Append to array, expanding if necessary */
            if (values_len == reserve)
            {
                reserve *= 2;
                seq_t *tmp = (seq_t *)realloc(values, reserve * sizeof(seq_t));
                if (!tmp) return; /* allocation failure, abort */
                values = tmp;
            }
            values[values_len++] = val;
        }
    }

    /* Sort the array in ascending order */
    qsort(values, values_len, sizeof(seq_t), cmp_seq_t);
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but safe return */

    /* Generate values on first call */
    generate_all();

    /* Bounds: if n exceeds the number of generated terms, return 0 (unlikely for intended n). */
    if ((size_t)n > values_len) return (seq_t)0;
    return values[n - 1];
}
