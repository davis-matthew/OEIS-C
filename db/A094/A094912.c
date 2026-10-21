/*
 * A094912  Output from a certain finite automaton when fed binary
 *          representation of n read from right to left.
 * https://oeis.org/A094912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Automaton: start in state A
   A,0 -> A
   A,1 -> B
   B,0 -> C
   B,1 -> A
   C,0 -> A
   C,1 -> C
   Output 1 if final state is C, otherwise 0 */
seq_t a(idx_t n)
{
    int state = 0; /* 0=A, 1=B, 2=C */
    while (n > 0)
    {
        int bit = n & 1;
        switch (state)
        {
            case 0: /* state A */
                state = bit ? 1 : 0;
                break;
            case 1: /* state B */
                state = bit ? 0 : 2;
                break;
            case 2: /* state C */
                state = bit ? 2 : 0;
                break;
        }
        n >>= 1;
    }
    return (seq_t)((state == 2) ? 1 : 0);
}
