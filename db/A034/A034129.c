/*
 * A034129  Decimal part of cube root of a(n) starts with 3: first term of
 *          runs.
 * https://oeis.org/A034129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil( ((10*(n+1)+3)^3) / 1000 )
 * Range:     u8 none, u16 n <= 2, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t and compute the base */
    seq_t base = (seq_t)n + (seq_t)1;   /* n + 1 */
    base *= (seq_t)10;                  /* 10*(n + 1) */
    base += (seq_t)3;                   /* + 3 */

    /* Compute the cube of the base */
    seq_t cube = base * base * base;    /* (10*(n+1)+3)^3 */

    /* Apply the ceil operation by adding 999 before dividing by 1000 */
    cube += (seq_t)999;                 /* + 999 */
    return cube / (seq_t)1000;          /* / 1000 */
}
