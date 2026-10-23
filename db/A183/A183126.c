/*
 * A183126  Toothpick sequence with toothpicks connected by their endpoints.
 * https://oeis.org/A183126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A183126 – Toothpick sequence
    a(0) = 0, a(1) = 1
    a(n) = 7 + 4 * ( (n-2) + Σ_{k=1}^{n-2} 3^{popcount(k)} ),  n ≥ 2
    where popcount(k) counts the 1‑bits in k.  */

#include <stdint.h>

/*  compute 3^e  – e is small (≤ 64)  */
static inline seq_t pow3_small(int e)
{
    seq_t r = 1;
    for (int i = 0; i < e; ++i) r *= 3;
    return r;
}

/*  popcount for an unsigned 64‑bit integer  */
static inline unsigned popcount_uint64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    /*  fall‑back implementation (slow but correct)  */
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

/*  main sequence function  */
seq_t a(idx_t n)
{
    /*  offset: a(0)=0, a(1)=1  */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /*  n ≥ 2  */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n - 2; ++k)
    {
        unsigned pc = popcount_uint64(k);
        sum += pow3_small((int)pc);
    }

    seq_t add = (seq_t)(n - 2) + sum;
    return (seq_t)7 + (seq_t)4 * add;
}
