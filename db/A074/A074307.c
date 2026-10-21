/*
 * A074307  Square of permutation in A074305.
 * https://oeis.org/A074307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = based on n mod 9: a(9k)=4k, a(9k+1)=8k+2, a(9k+2)=16k+3,
 *            a(9k+3)=8k+1, a(9k+4)=16k+11, a(9k+5)=16k+5, a(9k+6)=8k+7,
 *            a(9k+7)=16k+13, a(9k+8)=8k+6
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 9;
    seq_t kk = (seq_t)k; /* cast to seq_t for arithmetic */
    switch (n % 9) {
    case 0: return 4 * kk;
    case 1: return 8 * kk + 2;
    case 2: return 16 * kk + 3;
    case 3: return 8 * kk + 1;
    case 4: return 16 * kk + 11;
    case 5: return 16 * kk + 5;
    case 6: return 8 * kk + 7;
    case 7: return 16 * kk + 13;
    case 8: return 8 * kk + 6;
    }
    /* unreachable */
    return 0;
}
