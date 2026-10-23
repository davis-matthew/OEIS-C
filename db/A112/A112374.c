/*
 * A112374  Let T(n) = A000078(n+2), n >= 1; a(n) = smallest k such that n
 *          divides T(k).
 * https://oeis.org/A112374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k such that n divides T(k), where T(1)=1,
 *            T(2)=1, T(3)=2, T(4)=4 and T(k)=T(k-1)+T(k-2)+T(k-3)+T(k-4) for
 *            k≥5 (tetranacci numbers)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* special case: n = 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t mod = (seq_t)n;

    /* initial tetranacci residues modulo n */
    seq_t r1 = ((seq_t)1) % mod;   /* T(1) */
    if (r1 == (seq_t)0)            /* n = 1 handled above */
        return (seq_t)1;

    seq_t r2 = ((seq_t)1) % mod;   /* T(2) */
    seq_t r3 = ((seq_t)2) % mod;   /* T(3) */
    if (r3 == (seq_t)0)
        return (seq_t)3;
    seq_t r4 = ((seq_t)4) % mod;   /* T(4) */
    if (r4 == (seq_t)0)
        return (seq_t)4;

    /* iterate tetranacci modulo n until a zero residue is found */
    for (idx_t k = (idx_t)5; ; ++k)
    {
        seq_t sum = r1 + r2 + r3 + r4;   /* sum < 4*n <= 4*mod */
        seq_t r = sum % mod;
        if (r == (seq_t)0)
            return (seq_t)k;
        r1 = r2;
        r2 = r3;
        r3 = r4;
        r4 = r;
    }
}
