/*
 * A117795  Heptagonal numbers divisible by 7.
 * https://oeis.org/A117795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = H(k) where k is the n‑th integer such that k mod 7 is 0
 *            or 2, i.e. k = 7*((n-1)/2) for odd n and k = 7*(n/2)-5 for even
 *            n; H(k) = k*(5k-3)/2
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Heptagonal numbers divisible by 7 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t idx = (seq_t)n;
    seq_t k, n_val;

    if (idx & 1) {          /* n is odd -> k ≡ 0 (mod 7) */
        k = (idx - 1) / 2;
        n_val = 7 * k;
    } else {                /* n is even -> k ≡ 2 (mod 7) */
        k = idx / 2;
        n_val = 7 * k - 5;
    }

    /* H(n_val) = n_val * (5*n_val-3) / 2 */
    seq_t result = n_val * ((seq_t)5 * n_val - (seq_t)3) / (seq_t)2;
    return result;
}
