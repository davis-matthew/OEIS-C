/*
 * A080116  Characteristic function of A014486. a(n) = 1 if n's binary
 *          expansion is totally balanced, otherwise zero.
 * https://oeis.org/A080116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=1 if the binary representation of n forms a balanced
 *            parenthesis string (Dyck word), otherwise 0. (treat 1 as '(' and
 *            0 as ')', and a(n)=1 for the empty string n=0)
 * Range:     u8 n <= 9988, u16 n <= 9988, u32 n <= 9988, u64 n <= 9988,
 *            u128 n <= 9988, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Empty string is valid */
    if (n == 0)
        return (seq_t)1;

    /* Find the index of the most significant bit (0‑based). */
    int msb = -1;
    idx_t tmp = n;
    while (tmp) {
        msb++;
        tmp >>= 1;
    }

    int balance = 0;
    for (int i = msb; i >= 0; --i) {
        if (n & ((idx_t)1 << i))
            ++balance;   /* '('  */
        else
            --balance;   /* ')'  */
        if (balance < 0)
            return (seq_t)0;   /* prefix has more ')' than '(' */
    }
    return (balance == 0) ? (seq_t)1 : (seq_t)0;
}
