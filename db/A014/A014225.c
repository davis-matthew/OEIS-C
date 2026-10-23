/*
 * A014225  Number of initial pieces needed to reach level n in the Solitaire
 *          Army game.
 * https://oeis.org/A014225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014225: Number of initial pieces needed to reach level n in the Solitaire Army game.
   Known that level 4 is the highest level that can be reached.
   For n <= 4: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=20.
   For n > 4: the level is unattainable, so return 0. */

seq_t a(idx_t n) {
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        case 3: return (seq_t)8;
        case 4: return (seq_t)20;
        default: return (seq_t)0;
    }
}
