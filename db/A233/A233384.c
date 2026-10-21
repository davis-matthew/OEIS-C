/*
 * A233384  Positive integer solutions (x, y, z), with x <= y <= z, to the
 *          equation x*y*z = x + y + z + 2.
 * https://oeis.org/A233384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate all positive integer solutions (x, y, z), with x <= y <= z,
   to the equation x*y*z = x + y + z + 2.
   The sequence lists the coordinates of each solution in lexicographic
   order (x, y, z) repeated cyclically. */
#include <stdint.h>

seq_t a(idx_t n)
{
    typedef struct { seq_t x, y, z; } triple_t;
    static int ready = 0;
    static triple_t triples[3];
    if (!ready) {
        int count = 0;
        for (int a = 0; a <= 4; ++a) {
            for (int b = a; b <= 4; ++b) {
                for (int c = b; c <= 4; ++c) {
                    seq_t A = (seq_t)a;
                    seq_t B = (seq_t)b;
                    seq_t C = (seq_t)c;
                    seq_t left = A*B*C + A*B + B*C + C*A;
                    if (left == (seq_t)4) {
                        triples[count].x = A + (seq_t)1;
                        triples[count].y = B + (seq_t)1;
                        triples[count].z = C + (seq_t)1;
                        ++count;
                    }
                }
            }
        }
        ready = 1;
    }
    idx_t idx0 = n - (idx_t)1;
    idx_t triple_idx = (idx0 / (idx_t)3) % (idx_t)3;
    idx_t pos = idx0 % (idx_t)3;
    seq_t val;
    switch (pos) {
        case 0: val = triples[(int)triple_idx].x; break;
        case 1: val = triples[(int)triple_idx].y; break;
        case 2: val = triples[(int)triple_idx].z; break;
        default: val = 0; break; /* unreachable */
    }
    return val;
}
