/*
 * A056022  Numbers k such that k^6 == 1 (mod 7^2).
 * https://oeis.org/A056022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=18, a(3)=19, a(4)=30, a(5)=31, a(6)=48,
 *            a(n)=a(n-6)+49 for n>6 (equivalently
 *            a(n)=a((n-1)%6)+49*((n-1)/6))
 * Range:     u8 n <= 31, u16 n <= 8025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined only for n>=1, below offset irrelevant */
    static const seq_t off[6] = {
        (seq_t)1,
        (seq_t)18,
        (seq_t)19,
        (seq_t)30,
        (seq_t)31,
        (seq_t)48
    };
    idx_t idx = (n - 1) % 6;               /* position within one 6‑step cycle */
    idx_t k   = (n - 1) / 6;               /* number of complete 6‑step cycles */

    seq_t res = off[idx];
    unsigned __int128 prod = (unsigned __int128)49 * k; /* avoid overflow in the intermediate */
    seq_t add = (seq_t)prod;
    res += add;
    return res;
}
