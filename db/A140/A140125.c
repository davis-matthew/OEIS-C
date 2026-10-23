/*
 * A140125  A prime chain of 147 terms consisting of the output of four
 *          equations that alternate sequentially. The equations are either
 *          subsequences of x^2 - 79*x + 1601 or transforms. The four
 *          equations are 4*x^2 - 146*x + 1373, 4*x^2 - 144*x + 1459, 4*x^2 -
 *          142*x + 1301, 4*x^2 - 140*x + 1877.
 * https://oeis.org/A140125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less, easy
 * Formula:   a(4n+1)=4*n^2-146*n+1373, a(4n+2)=4*n^2-144*n+1459, *
 *            a(4n+3)=4*n^2-142*n+1301, a(4n+4)=4*n^2-140*n+1877
 * Range:     u8 none, u16 n <= 147, u32 n <= 147, u64 n <= 147, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behavior for indices below the offset */

    idx_t nn = n - 1;                      /* convert to 0‑based index */
    idx_t block = nn / 4;                  /* k = floor((n-1)/4) */
    seq_t k = (seq_t)block;
    int mod = nn % 4;                      /* remainder 0..3 */

    switch (mod) {
        case 0: /* a(4k+1) */
            return (seq_t)4 * k * k - (seq_t)146 * k + (seq_t)1373;
        case 1: /* a(4k+2) */
            return (seq_t)4 * k * k - (seq_t)144 * k + (seq_t)1459;
        case 2: /* a(4k+3) */
            return (seq_t)4 * k * k - (seq_t)142 * k + (seq_t)1301;
        case 3: /* a(4k+4) */
            return (seq_t)4 * k * k - (seq_t)140 * k + (seq_t)1877;
    }
    return 0;   /* unreachable, but keeps the compiler happy */
}
