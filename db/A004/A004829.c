/*
 * A004829  Numbers that are the sum of at most 7 positive cubes.
 * https://oeis.org/A004829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve of minimal cube counts up to 2000 marks numbers that are
 *            sums of at most 7 positive cubes; by Siksek every n > 454
 *            qualifies, so beyond the sieve a(n) = n - 1 + E, where E is the
 *            count of excluded numbers
 * Range:     u8 n <= 244, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#define A4829_LIMIT 2000

static long long a4829_members[A4829_LIMIT + 1];
static long long a4829_count = 0;
static long long a4829_excluded = 0;
static bool a4829_ready = false;

static void a4829_init(void) {
    static int mincubes[A4829_LIMIT + 1];
    int x, c;
    if (a4829_ready) return;
    mincubes[0] = 0;
    for (x = 1; x <= A4829_LIMIT; x++) {
        int best = 1000000;
        for (c = 1; c * c * c <= x; c++) {
            int v = mincubes[x - c * c * c] + 1;
            if (v < best) best = v;
        }
        mincubes[x] = best;
    }
    a4829_count = 0;
    a4829_excluded = 0;
    for (x = 0; x <= A4829_LIMIT; x++) {
        if (mincubes[x] <= 7)
            a4829_members[a4829_count++] = x;
        else
            a4829_excluded++;
    }
    a4829_ready = true;
}

seq_t a(idx_t n) {
    a4829_init();
    if (n < 1) return (seq_t)0;
    if (n <= (idx_t)a4829_count)
        return (seq_t)a4829_members[n - 1];
    return (seq_t)(n - 1) + (seq_t)a4829_excluded;
}
