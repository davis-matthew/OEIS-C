/*
 * A108804  Self-convolution of A010060 (Thue-Morse sequence).
 * https://oeis.org/A108804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{n} T(k)·T(n−k), where T(k) is the Thue–Morse
 *            sequence (T(k)=0 if popcount(k) is even, otherwise T(k)=1). The
 *            implementation uses the C built‑in parity function to test the
 *            parity of popcount(k)
 * Range:     u8 n <= 638, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        unsigned long long ui = (unsigned long long)i;
        unsigned long long uj = (unsigned long long)(n - i);
        /* Parity of popcount: 1 if odd number of set bits, 0 otherwise. */
        unsigned t1 = __builtin_parityll(ui);
        unsigned t2 = __builtin_parityll(uj);
        sum += (seq_t)(t1 & t2);
    }
    return sum;
}
