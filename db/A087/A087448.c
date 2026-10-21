/*
 * A087448  a(n) = 3^(n-1)*(n+3)/2 - (n-1)/2.
 * https://oeis.org/A087448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^(n-1)*(n+3) - (n-1))/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* For seq_t that fits in 64 bits we use a 128‑bit intermediate
       to avoid overflow of the numerator. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 pow3 = 1;
        for (idx_t i = 1; i < n; ++i) {
            pow3 *= (unsigned __int128)3;
        }
        unsigned __int128 numerator =
            pow3 * ((unsigned __int128)n + (unsigned __int128)3)
            - ((unsigned __int128)n - (unsigned __int128)1);
        return (seq_t)(numerator / (unsigned __int128)2);
    }

    /* For larger seq_t types (128‑bit or arbitrary‑precision) we
       perform the computation with seq_t operations only. */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow3 *= (seq_t)3;
    }
    seq_t numerator =
        pow3 * ((seq_t)n + (seq_t)3) - ((seq_t)n - (seq_t)1);
    return numerator / (seq_t)2;
}
