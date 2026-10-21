/*
 * A204018  Symmetric matrix based on f(i,j)=1+max(j mod i, i mod j), by
 *          antidiagonals.
 * https://oeis.org/A204018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value of the symmetric matrix M(i,j)=1+max(j mod i,i
 *            mod j) enumerated by anti‑diagonals with index offset 1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static uint64_t floor_sqrt_u128(unsigned __int128 x)
{
    long double approx = sqrtl((long double)x);
    unsigned __int128 r = (unsigned __int128)(approx);
    while ((r + 1) * (r + 1) <= x)
        ++r;
    while (r * r > x)
        --r;
    return (uint64_t)r;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N == 0) return 0; /* undefined for n=0, but return 0 */

    /* Find diagonal number d such that d(d+1)/2 >= N */
    unsigned __int128 mult = (unsigned __int128)N * 8 + 1;
    uint64_t sqrt_term = floor_sqrt_u128(mult);
    uint64_t r = (sqrt_term - 1) / 2;
    uint64_t d = r;
    if (((unsigned __int128)r * (r + 1)) / 2 < N)
        d = r + 1;

    /* Position within the diagonal */
    uint64_t prev = (unsigned __int128)(d - 1) * d / 2;
    uint64_t pos = N - prev;

    uint64_t i = pos;
    uint64_t j = d + 1 - pos;

    uint64_t mod1 = j % i;
    uint64_t mod2 = i % j;
    uint64_t maxmod = mod1 > mod2 ? mod1 : mod2;

    return (seq_t)(maxmod + 1);
}
