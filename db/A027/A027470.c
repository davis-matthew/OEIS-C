/*
 * A027470  a(n) = 225*(n-1)*(n-2)/2.
 * https://oeis.org/A027470
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*(n-1)*(n-2)/2 Offset: 3 (so a(3) is the first term
 *            listed below) a(n) = 225*(n-1)*(n-2)/2, valid for n >= 3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if (nseq < 3u)
        return 0;          /* behavior below offset is unspecified */

    seq_t a1 = nseq - 1;
    seq_t a2 = nseq - 2;
    seq_t prod;

    /* compute (a1 * a2) / 2 without overflow when possible */
    if ((a1 & 1u) == 0u)
        prod = (a1 / 2u) * a2;
    else
        prod = (a2 / 2u) * a1;

    return 225u * prod;
}
