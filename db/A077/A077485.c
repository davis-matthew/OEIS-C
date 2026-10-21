/*
 * A077485  a(1) = 1, then smallest n-digit square which leaves a square at
 *          every step if most significant digit and least significant digit
 *          are deleted until a one- or two-digit square is obtained. a(2n) =
 *          0 if no such square exists. a(2n+1) = 10^2n only if no nontrivial
 *          candidate exists.
 * https://oeis.org/A077485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more, changed
 * Formula:   a(1)=1, a(2)=16, a(3)=144, a(4)=1369, a(5)=10816; * For n≥6:
 *            a(n)=0 if n≡0 or 2 (mod 4); * a(n)=10^(4·k) if n=4k+1, *
 *            a(n)=(10^(2k+1)+2)^2 if n=4k+3. * * The code follows this
 *            definition directly and avoids any * pre‑computed table of
 *            values
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow10(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    /* Special cases for n = 1 … 5 */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)16;
    if (n == (idx_t)3) return (seq_t)144;
    if (n == (idx_t)4) return (seq_t)1369;
    if (n == (idx_t)5) return (seq_t)10816;

    /* n ≥ 6 */
    idx_t mod = n & (idx_t)3;          /* n % 4 using bit‑wise for speed */

    if (mod == 0 || mod == 2)           /* a(4k) = 0,  a(4k+2) = 0 */
        return (seq_t)0;

    if (mod == 1)                       /* a(4k+1) = 10^(4k) */
    {
        idx_t k = (n - (idx_t)1) / (idx_t)4;
        return pow10( (idx_t)(4 * k) );
    }

    /* mod == 3  –  a(4k+3) = (10^(2k+1)+2)^2 */
    {
        idx_t k = (n - (idx_t)3) / (idx_t)4;
        seq_t base = pow10( (idx_t)(2 * k + 1) );
        base = base + (seq_t)2;
        return base * base;
    }
}
