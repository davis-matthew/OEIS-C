/*
 * A077486  Largest n-digit square which leaves a square at every step if most
 *          significant digit and least significant digit are deleted until a
 *          one- or two-digit square is obtained. a(2n) = 0 if no such square
 *          exists. a(2n+1) = 9*10^2n only if no nontrivial candidate exists.
 * https://oeis.org/A077486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 for n%4==0 or n%4==2; * a(n) = 9*10^(n-1) for n%4==1
 *            (n>=6), * a(n) = (2*10^((n-1)/2)+1)^2 for n%4==3 (n>=7), * with
 *            specific values for n=1..5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;
    if (n == 3) return (seq_t)841;
    if (n == 4) return (seq_t)8649;
    if (n == 5) return (seq_t)64009;

    /* n >= 6 */
    if ((n & 3) == 0 || (n & 3) == 2) /* n % 4 == 0 or 2 */
        return (seq_t)0;

    if ((n & 3) == 1) { /* n % 4 == 1 */
        seq_t r = pow10(n - 1);
        return r * (seq_t)9;
    }

    /* n % 4 == 3 */
    idx_t exp = (n - 1) / 2;
    seq_t ten = pow10(exp);
    seq_t val = ten * (seq_t)2 + (seq_t)1;
    return val * val;
}
