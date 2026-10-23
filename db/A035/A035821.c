/*
 * A035821  Coordination sequence for lattice D*_72 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k=0..72} 2^k * C(72, k) * C(n-1, k-1) + 2^72 * C((72+2*n)/2 - 1, 71)
 * (with the convention C(a,b)=0 if b<0 or b>a; C(-1,-1)=1)
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Constants */
    static const idx_t N = 72;
    static bool initialized = false;
    static seq_t binomN[73];     /* C(N, k) for k = 0..N */
    static seq_t pow2N;          /* 2^N */

    if (!initialized) {
        pow2N = pow2(N);
        for (idx_t k = 0; k <= N; ++k) {
            binomN[k] = binomial(N, k);
        }
        initialized = true;
    }

    /* First summation term */
    seq_t sum = 0;
    idx_t maxk = n < N ? n : N;  /* because C(n-1, k-1) is 0 for k>n  */
    for (idx_t k = 1; k <= maxk; ++k) {
        /* binomN[k] * 2^k */
        seq_t term = binomN[k] * pow2(k);
        /* multiply by C(n-1, k-1) */
        term = term * binomial(n - 1, k - 1);
        sum += term;
    }
    /* Note: k == 0 contributes only when n==0, handled separately */

    /* Second term */
    idx_t t = (N + n + n) / 2 - 1;            /* (N + 2*n)/2 - 1 */
    seq_t second = pow2N * binomial(t, N - 1);

    return sum + second;
}
