/*
 * A164475  Number of binary strings of length n with no substrings equal to
 *          0001 0101 or 1110.
 * https://oeis.org/A164475
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 61, u64 n <= 128,
 *            u128 n <= 261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A164475: number of binary strings of length n that avoid the
   forbidden substrings 0001, 0101, and 1110.
   The function uses a deterministic finite automaton with 10 states.
   Transition table explained in the code comments.
*/
seq_t a(idx_t n)
{
    /* Mapping of states:
       0: ""    (empty prefix)
       1: "0"
       2: "1"
       3: "00"
       4: "01"
       5: unused (reserved for symmetry)
       6: "11"
       7: "000"
       8: "010"
       9: "111"
    */
    static const int trans[10][2] = {
        /*0*/ { 1,  2},   /* ""    */
        /*1*/ { 3,  4},   /* "0"   */
        /*2*/ { 1,  6},   /* "1"   */
        /*3*/ { 7,  4},   /* "00"  */
        /*4*/ { 8,  6},   /* "01"  */
        /*5*/ { -1, -1},  /* unused */
        /*6*/ { 1,  9},   /* "11"  */
        /*7*/ { 7, -1},   /* "000" */
        /*8*/ { 3, -1},   /* "010" */
        /*9*/ { -1, 9}    /* "111" */
    };
    const int terminal = -1;

    seq_t curr[10] = {0};
    seq_t next[10] = {0};

    curr[0] = (seq_t)1;          /* start from the empty prefix */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 10; ++i) next[i] = (seq_t)0;

        for (int s = 0; s < 10; ++s) {
            if (curr[s] == (seq_t)0) continue;
            const seq_t val = curr[s];
            for (int b = 0; b < 2; ++b) {
                int ns = trans[s][b];
                if (ns == terminal) continue;   /* forbidden transition */
                next[ns] += val;
            }
        }

        for (int i = 0; i < 10; ++i) curr[i] = next[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 10; ++i) total += curr[i];
    return total;
}
