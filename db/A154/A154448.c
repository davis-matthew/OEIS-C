/*
 * A154448  Permutation of nonnegative integers induced by wreath recursion
 *          a=s(b,c), b=s(c,a), c=(c,c), starting from state a, rewriting bits
 *          from the second most significant bit toward the least significant
 *          end.
 * https://oeis.org/A154448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Permutation of nonnegative integers defined by the automaton
   a = s(b,c),  b = s(c,a),  c = (c,c).
   Starting in state a, the bits of n are processed from the
   second most significant bit down to the least significant bit.
   For each processed bit:
     * if the current state is a or b, the bit is complemented
       (0 ↦ 1, 1 ↦ 0);
     * if the current state is c, the bit is left unchanged.
   The state then updates as follows:
     a: 0 → b, 1 → c
     b: 0 → c, 1 → a
     c: always stays c
   The final binary string (obtained by modifying the input bits as
   described) interpreted as an unsigned integer is a(n).  */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;          /* start with the original bits      */

    /* find position of most significant 1‑bit (floor(log2(n))) */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1)                /* msb records the exponent      */
        ++msb;

    int state = 0;                   /* 0 = a, 1 = b, 2 = c            */

    /* process bits from second most significant down to least */
    for (idx_t i = msb; i > 0; --i) {
        idx_t bitpos = i - 1;            /* current bit index          */
        int  x = (int)((n >> bitpos) & 1);   /* input bit at this position */

        /* modify the output bit according to the current state */
        if (state != 2)                 /* states a or b complement */
            res ^= ((seq_t)1 << bitpos);

        /* update state after reading this bit */
        if (state == 0) {                /* state a   */
            state = (x == 0) ? 1 : 2;    /* 0 → b, 1 → c */
        } else if (state == 1) {         /* state b   */
            state = (x == 0) ? 2 : 0;    /* 0 → c, 1 → a */
        }                                 /* state c stays c */
    }

    return res;
}
