/*
 * A074842  Triplets: base 10 representation is the juxtaposition of three
 *          identical strings.
 * https://oeis.org/A074842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of n three times – i.e. if k =
 *            floor(log10(n)) + 1, a(n) = n * 10^(2k) + n * 10^k + n
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* count digits of n */
    idx_t t = n;
    int k = 0;
    while (t > 0) {
        k++;
        t /= 10;
    }

    /* compute 10^k as seq_t */
    seq_t pow10k = 1;
    for (int i = 0; i < k; ++i) {
        pow10k *= 10;
    }

    /* compute 10^(2k) */
    seq_t pow20k = pow10k * pow10k;

    /* cast n to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;

    /* a(n) = n * 10^(2k) + n * 10^k + n */
    seq_t res = nseq * pow20k + nseq * pow10k + nseq;
    return res;
}
