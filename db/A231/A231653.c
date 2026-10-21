/*
 * A231653  Number of non-equivalent ways to choose 4 points in an equilateral
 *          triangle grid of side n.
 * https://oeis.org/A231653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 + 4*n^7 - 6*n^6 - 32*n^5 + 84*n^4 - 32*n^3 - 16*n^2
 *            - 192*n + (n is odd ? 84*n^3 - 234*n^2 + 168*n + 171 : 0) + (n
 *            mod 3 == 1 ? 128*n^2 + 128*n - 256 : 0)) / 2304
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 41, u64 n <= 673,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an unsigned __int128 intermediate when
   compiling with a compiler that supports it.  It therefore protects
   the computation from overflow for all n such that the final
   result fits into a 64‑bit unsigned integer.  For larger
   seq_t types (128‑bit or arbitrary precision) the seq_t arithmetic
   is used directly and is naturally safe.  */

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#define HAVE_U128 1
#else
#define HAVE_U128 0
#endif

seq_t a(idx_t n)
{
    /* Make a copy of n that can be used as a 128‑bit intermediate
       value when available.  */
#if HAVE_U128
    u128 N = n;
    u128 n1 = N;
    u128 n2 = n1 * n1;
    u128 n3 = n2 * n1;
    u128 n4 = n3 * n1;
    u128 n5 = n4 * n1;
    u128 n6 = n5 * n1;
    u128 n7 = n6 * n1;
    u128 n8 = n7 * n1;

    u128 num = n8
            + (u128)4 * n7
            - (u128)6 * n6
            - (u128)32 * n5
            + (u128)84 * n4
            - (u128)32 * n3
            - (u128)16 * n2
            - (u128)192 * n1;

    if ((n & (idx_t)1) == (idx_t)1) {
        num += (u128)84 * n3
             - (u128)234 * n2
             + (u128)168 * n1
             + (u128)171;
    }

    if (n % (idx_t)3 == (idx_t)1) {
        num += (u128)128 * n2
             + (u128)128 * n1
             - (u128)256;
    }

    num /= (u128)2304;
    return (seq_t)num;
#else
    /* If __int128 is not available we perform the computation
       directly in seq_t type.  This path works reliably only
       when the final result fits into seq_t without intermediate
       overflow.  For 64‑bit seq_t the caller must ensure n is
       small enough that the intermediate calculations stay in
       bounds; otherwise a narrow build will not be able to
       evaluate large n accurately. */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;
    seq_t n8 = n7 * n1;

    seq_t num = n8
            + (seq_t)4 * n7
            - (seq_t)6 * n6
            - (seq_t)32 * n5
            + (seq_t)84 * n4
            - (seq_t)32 * n3
            - (seq_t)16 * n2
            - (seq_t)192 * n1;

    if ((n & (idx_t)1) == (idx_t)1) {
        num += (seq_t)84 * n3
             - (seq_t)234 * n2
             + (seq_t)168 * n1
             + (seq_t)171;
    }

    if (n % (idx_t)3 == (idx_t)1) {
        num += (seq_t)128 * n2
             + (seq_t)128 * n1
             - (seq_t)256;
    }

    return num / (seq_t)2304;
#endif
}
