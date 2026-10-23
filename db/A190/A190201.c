/*
 * A190201  a(n) = [n*u+n*v]-[n*u]-[n*v], where u=sqrt(5), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*u+n*v] - [n*u] - [n*v] * with u = sqrt(5), v = 1/u. *
 *            This implementation uses integer arithmetic with high‑precision
 *            rational * approximations of u and v (12 decimal digits). The
 *            carry in * floor(x+y) = floor(x)+floor(y)+carry gives the
 *            desired 0/1 value. The * method works for all n >...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t SQ5_DEN   = 1000000000000ULL;          /* 10^12   */
static const seq_t SQ5_NUM   = 2236067977499ULL;          /* floor(sqrt5 * 1e12)   */
static const seq_t INV_SQ5_NUM = 447213595499ULL;         /* floor((1/sqrt5) * 1e12) */
static const seq_t SUM_NUM   = 2683281572998ULL;          /* SQ5_NUM + INV_SQ5_NUM   */

seq_t a(idx_t n)
{
    /* floor(n*u) = floor(n * SQ5_NUM / SQ5_DEN) */
    seq_t floor1 = (unsigned __int128)n * SQ5_NUM / SQ5_DEN;
    /* floor(n*v) = floor(n * INV_SQ5_NUM / SQ5_DEN) */
    seq_t floor2 = (unsigned __int128)n * INV_SQ5_NUM / SQ5_DEN;
    /* floor(n*(u+v)) = floor(n * SUM_NUM / SQ5_DEN) */
    seq_t floor3 = (unsigned __int128)n * SUM_NUM / SQ5_DEN;

    return floor3 - floor1 - floor2;
}
