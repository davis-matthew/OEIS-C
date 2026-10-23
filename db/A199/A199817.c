/*
 * A199817  Odd numbers producing 10 even numbers in the Collatz iteration.
 * https://oeis.org/A199817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   odd numbers producing exactly 10 even numbers in the Collatz
 *            iteration. This code enumerates odd integers until it finds the
 *            four terms of the finite sequence A199817 and then returns the
 *            nth term from that list, with a(n)=0 for n>4
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t seq_a[4];
static size_t seq_len = 0;
static int seq_init_done = 0;

static void init_seq(void)
{
    if (seq_init_done)
        return;
    seq_init_done = 1;
    seq_len = 0;

    seq_t candidate = 1;
    while (seq_len < 4) {
        if ((candidate & 1) != 0) {          // only odd candidates
            seq_t x = candidate;
            size_t cnt = 0;
            while (x != 1) {
                if ((x & 1) == 0) {          // even step
                    x >>= 1;
                    cnt++;
                } else {                      // odd step
                    x = 3 * x + 1;
                }
            }
            if (cnt == 10)
                seq_a[seq_len++] = candidate;
        }
        candidate++;
    }
}

seq_t a(idx_t n)
{
    init_seq();
    if (n == 0 || (size_t)n > seq_len)
        return (seq_t)0;
    return seq_a[(size_t)n - 1];
}
