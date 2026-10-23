/*
 * A022538  Nexus numbers (n+1)^22 - n^22.
 * https://oeis.org/A022538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^22 - n^22
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 6, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = Σ_{k=0}^{21} C(22,k) * n^k  */

static const unsigned long long C[22] = {
    1ULL,    22ULL,   231ULL,   1540ULL,  7315ULL,
   26334ULL, 74613ULL,170544ULL,319770ULL,497420ULL,
   646646ULL,705432ULL,646646ULL,497420ULL,319770ULL,
   170544ULL,74613ULL,26334ULL,7315ULL,1540ULL,231ULL,22ULL
};

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t pow   = 1;   /* n^0 */
    seq_t sum   = 0;

    for (idx_t k = 0; k < 22; ++k) {
        /* term = C(22,k) * n^k */
        sum += (seq_t)C[k] * pow;
        pow *= n_seq;      /* prepare next power n^{k+1} */
    }

    return sum;
}
