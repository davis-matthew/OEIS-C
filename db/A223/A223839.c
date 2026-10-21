/*
 * A223839  Number of 3 X n 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 3 n^5 + 145 n^4 + 165 n^3 + 2014 n^2 - 888 n +
 *            1440) / 720, for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 120, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use 128-bit arithmetic if supported, otherwise fallback to 64-bit (may overflow for large n). */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
typedef __int128 int128_t;
#  define HAVE_INT128 1
#else
typedef long long int128_t;
#  define HAVE_INT128 0
#endif

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety we return 0 for n = 0. */
    if (n == 0)
        return 0;

    /* Compute the polynomial expression using 128‑bit integer arithmetic. */
    int128_t t   = (int128_t)n;
    int128_t t2  = t * t;
    int128_t t3  = t2 * t;
    int128_t t4  = t3 * t;
    int128_t t5  = t4 * t;
    int128_t t6  = t5 * t;

    int128_t numerator =
        t6
        + (int128_t)3   * t5
        + (int128_t)145 * t4
        + (int128_t)165 * t3
        + (int128_t)2014 * t2
        - (int128_t)888 * t
        + (int128_t)1440;

    /* The division is exact because the sequence values are integer. */
    int128_t result = numerator / (int128_t)720;

    return (seq_t)result;
}
