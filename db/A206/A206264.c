/*
 * A206264  Number of (n+1) X 6 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A206264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11520 + 8028*n + 5104*n^2 + 1665*n^3 + 295*n^4 + 27*n^5
 *            + n^6) / 720 for n>1 even, a(n) = (10800 + 8028*n + 5104*n^2 +
 *            1665*n^3 + 295*n^4 + 27*n^5 + n^6) / 720 for n>1 odd. a(1) = 72
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 116, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused, safety for n<1 */
    if (n == 1) return 72;  /* first term of the sequence */

    /* Use 128‑bit arithmetic for intermediate computations. */
    unsigned __int128 N = n;
    unsigned __int128 num = 0;

    num += N * N * N * N * N * N;                            /* n^6 */
    num += (unsigned __int128)27 * N * N * N * N * N;        /* 27 n^5 */
    num += (unsigned __int128)295 * N * N * N * N;           /* 295 n^4 */
    num += (unsigned __int128)1665 * N * N * N;              /* 1665 n^3 */
    num += (unsigned __int128)5104 * N * N;                  /* 5104 n^2 */
    num += (unsigned __int128)8028 * N;                      /* 8028 n */
    num += (n & 1) ? (unsigned __int128)10800
                   : (unsigned __int128)11520;               /* 10800 or 11520 */

    num /= 720;   /* final division, result is integer */

    return (seq_t)num;   /* truncate if larger than seq_t for 64‑bit builds */
}
