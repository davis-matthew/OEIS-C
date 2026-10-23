/*
 * A190588  Losing positions in Nim (misere) with up to 9 stones on each heap.
 * https://oeis.org/A190588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A190588: all digit–nondecreasing numbers that represent
 * losing positions in misère Nim with heaps of size 1–9.
 * Each call to a(idx_t n) returns a(n) for n ≥ 1.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;  /* store computed terms */
static size_t cap = 0;       /* capacity of cache */
static size_t used = 0;      /* number of terms stored */
static int curr_len = 1;     /* next length to generate */

/* Recursively enumerate all nondecreasing digit strings of length L.
 * pos      - current position (0 .. L-1)
 * start    - minimum digit allowed at this position
 * val      - numeric value built so far
 * nimSum   - XOR of all digits seen so far
 * allOnes  - whether all seen digits are 1
 */
static void generate_len_rec(int pos, int L, int start, seq_t val,
                             int nimSum, int allOnes)
{
    for (int d = start; d <= 9; ++d) {
        seq_t nextVal = val * 10 + (seq_t)d;
        int nextSum = nimSum ^ d;
        int nextAll = allOnes && (d == 1);

        if (pos == L - 1) {
            int keep = 0;
            if (nextAll) {
                keep = (L & 1) != 0;   /* odd number of heaps of size 1 */
            } else {
                keep = (nextSum == 0); /* normal Nim sum zero */
            }
            if (keep) {
                if (used == cap) {
                    size_t newCap = cap ? cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
                    cap = newCap;
                }
                cache[used++] = nextVal;
            }
        } else {
            generate_len_rec(pos + 1, L, d, nextVal, nextSum, nextAll);
        }
    }
}

static void generate_len(int L)
{
    generate_len_rec(0, L, 1, (seq_t)0, 0, 1);
}

seq_t a(idx_t n)
{
    while (used < n) {
        generate_len(curr_len);
        ++curr_len;
    }
    return cache[n - 1];
}
