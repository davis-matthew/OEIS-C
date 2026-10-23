/*
 * A159986  Catalan numbers read modulo 7.
 * https://oeis.org/A159986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n) mod 7 Catalan(n) = binomial(2n, n) / (n+1)
 *            Here we use the equivalent identity a(n) ≡ binomial(2n, n) –
 *            binomial(2n, n+1) (mod 7) computed by Lucas theorem for the
 *            prime 7
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial coefficients modulo 7 for 0 ≤ n,k ≤ 6 */
static const seq_t _C7[7][7] = {
    { 1, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 0 },
    { 1, 2, 1, 0, 0, 0, 0 },
    { 1, 3, 3, 1, 0, 0, 0 },
    { 1, 4, 6, 4, 1, 0, 0 },
    { 1, 5, 3, 3, 5, 1, 0 },   /* 4Ck mod 7: 4,6,4 => 4,6,4 mod7 = 4,6,4 */
    { 1, 6, 1, 6, 1, 6, 1 }    /* 5Ck mod 7: 5,10,10,5 => 5,3,3,5 mod7 */
};

/* Lucas theorem: binomial(N,K) mod 7 for arbitrary N,K (idx_t). */
static seq_t _binom_mod_7(idx_t N, idx_t K) {
    if (K > N) return 0;
    seq_t res = 1;
    while (N) {
        uint32_t ni = (uint32_t)(N % 7);
        uint32_t ki = (uint32_t)(K % 7);
        if (ki > ni) return 0;
        res = (res * _C7[ni][ki]) % 7;
        N /= 7;
        K /= 7;
    }
    return res;
}

/* Main entry function for the sequence. */
seq_t a(idx_t n) {
    idx_t N = 2 * n;
    seq_t b1 = _binom_mod_7(N, n);        /* binom(2n,n) mod 7 */
    seq_t b2 = _binom_mod_7(N, n + 1);    /* binom(2n,n+1) mod 7 */
    seq_t res = b1 + 7 - b2;              /* ensure non‑negative before mod */
    res %= 7;
    return res;
}
