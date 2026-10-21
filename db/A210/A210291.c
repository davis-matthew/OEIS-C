/*
 * A210291  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant >n.
 * https://oeis.org/A210291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1}^{n} Σ_{d=1}^{n} #{(b,c) : 0≤b,c≤n, bc < ad – n}
 *            Computed via convolution of product‐distributions: let f(x) =
 *            #{(a,d) : 1≤a,d≤n, a·d = x}. let g(y) = #{(b,c) : 0≤b,c≤n, b·c =
 *            y}. Then a(n) = Σ_{x>0} f(x)·(g(0)+Σ_{0<y<x–n} f(y)) with
 *            g(0)=2n+1
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle trivial small n */
    if (n <= 1) return (seq_t)0;

    /* Convert n to int for array bounds (n <= 10000 in practice) */
    int nn = (int)n;
    size_t n2 = (size_t)nn * (size_t)nn;          /* maximum product a*d */

    /* Allocate array of frequencies for products a*d (1..n²) */
    uint16_t *freq = (uint16_t *)calloc(n2 + 1, sizeof(uint16_t));
    if (!freq) return (seq_t)0; /* allocation failure → return 0 */

    /* Build frequency array f(x) for a,d in [1,n] */
    for (int a = 1; a <= nn; ++a) {
        for (int d = 1; d <= nn; ++d) {
            size_t prod = (size_t)a * (size_t)d;
            ++freq[prod];
        }
    }

    /* g(0) = number of (b,c) with bc == 0: (b==0 or c==0) */
    uint64_t g0 = 2ULL * (uint64_t)nn + 1ULL;

    seq_t ans = (seq_t)0;
    uint64_t pref_t = 0;          /* Σ_{y < t} f(y)  (t will be x - n) */

    for (size_t x = 1; x <= n2; ++x) {
        size_t t = (x > (size_t)nn) ? (x - (size_t)nn) : 0ULL;   /* t = max(0, x-n) */
        uint64_t prefB = 0;
        if (t > 0) {
            prefB = g0 + pref_t;          /* Σ_{y < t} f(y) + g(0) */
        }

        ans += (seq_t)freq[x] * (seq_t)prefB;

        /* update pref_t for next iteration where t increases by 1 */
        if (t > 0 && t <= n2) {
            pref_t += freq[t];
        }
    }

    free(freq);
    return ans;
}
