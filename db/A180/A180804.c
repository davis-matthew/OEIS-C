/*
 * A180804  Number of distinct solutions of sum{i=1..2}(x(2i-1)*x(2i)) = 1
 *          (mod n), with x() in 0..n-1.
 * https://oeis.org/A180804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct solutions of * Σ_{i=1..2}
 *            (x(2i-1)*x(2i)) ≡ 1 (mod n), * with each x ∈ {0,1,…,n‑1}. *
 *            Distinctness is taken with respect to unordered pairs * (x1,x2)
 *            and (x3,x4) and to swapping the two pairs. * * Method: * 1.
 *            Count, for each residue r, the number f(r) of unordered * pai...
 * Range:     u8 n <= 12, u16 n <= 82, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* gcd for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main function */
seq_t a(idx_t n)
{
    /* special case n==1 */
    if (n == 1) {
        return (seq_t)0;
    }

    /* convenient 32/64 bit types for loops */
    uint64_t N = (uint64_t)n;

    /* allocate array f[0..N-1] */
    uint64_t *f = (uint64_t *)calloc(N, sizeof(uint64_t));
    if (!f) return (seq_t)0; /* out of memory: return 0 */

    /* count unordered pairs (a,b), a≤b */
    for (uint64_t a = 0; a < N; ++a) {
        for (uint64_t b = a; b < N; ++b) {
            uint64_t r = (a * b) % N;
            ++f[r];
        }
    }

    /* accumulate answer using unsigned __int128 to avoid overflow */
    unsigned __int128 ans = 0;
    for (uint64_t r = 0; r < N; ++r) {
        uint64_t s = (N + 1 - r) % N;   /* (1 - r) mod N */
        if (r < s) {
            ans += (unsigned __int128)f[r] * f[s];
        } else if (r == s) {
            ans += (unsigned __int128)f[r] * (f[r] + 1) / 2;
        }
    }

    free(f);
    return (seq_t)ans;
}
