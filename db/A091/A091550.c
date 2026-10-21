/*
 * A091550  Second column (k=3) sequence of array A091746 ((6,2)-Stirling2)
 *          divided by 12.
 * https://oeis.org/A091550
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n) * product_{j=0}^{n-1} (2*j+1) *
 *            (product_{j=0}^{n-1} (4*j+3) - 3*product_{j=0}^{n-1} (4*j+1))) /
 *            72 (offset: a(2) is the first term)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 int128_t;
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

seq_t a(idx_t n)
{
    /* Offset: a(2) is the first term. For n < 2 we return 0. */
    if (n < 2) return 0;

#if HAVE_INT128
    /* For small integral seq_t types compute with 128‑bit intermediates
       to avoid overflow for all n whose result fits in the seq_t size. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        int128_t pow2   = 1;
        int128_t prodOdd = 1;
        int128_t prod41  = 1;
        int128_t prod43  = 1;
        for (idx_t j = 0; j < n; ++j) {
            int128_t jj = j;
            prodOdd *= (2*jj + 1);
            prod41  *= (4*jj + 1);
            prod43  *= (4*jj + 3);
            pow2    *= (int128_t)2;
        }
        int128_t diff   = prod43 - 3*prod41;
        int128_t numer  = pow2 * prodOdd;
        numer          *= diff;
        numer          /= (int128_t)72;
        return (seq_t)numer;
    }
#endif

    /* General case: compute with whatever seq_t type the build uses. */
    seq_t pow2   = 1;
    seq_t prodOdd = 1;
    seq_t prod41  = 1;
    seq_t prod43  = 1;
    for (idx_t j = 0; j < n; ++j) {
        seq_t jj = (seq_t)j;
        prodOdd *= (seq_t)(2*jj + 1);
        prod41  *= (seq_t)(4*jj + 1);
        prod43  *= (seq_t)(4*jj + 3);
        pow2    *= (seq_t)2;
    }
    seq_t diff   = prod43 - (seq_t)3 * prod41;
    seq_t numer  = pow2 * prodOdd;
    numer      *= diff;
    numer      /= (seq_t)72;
    return numer;
}
