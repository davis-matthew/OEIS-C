/*
 * A028238  Weight distribution of (64,4096,28) Kerdock code.
 * https://oeis.org/A028238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weight distribution of the (64, 4096, 28) Kerdock code.
   a(n) = number of codewords of Hamming weight 4·n.
   The only non‑zero values are:
   a(0)  = 1   – the zero codeword.
   a(7)  = 1984
   a(8)  = 126
   a(9)  = 1984
   a(16) = 1   – the all‑ones codeword.
   All other indices return 0. */
seq_t a(idx_t n)
{
    switch (n) {
        case 0: case 16: return (seq_t)1;
        case 8:          return (seq_t)126;
        case 7: case 9:  return (seq_t)1984;
        default: return (seq_t)0;
    }
}
