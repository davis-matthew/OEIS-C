/*
 * A011682  A binary m-sequence: expansion of reciprocal of x^7+x+1.
 * https://oeis.org/A011682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) XOR a(n-7)
   Initial terms: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1
   Sequence is 127‑periodic */

#include <stdbool.h>
#include <stddef.h>

static seq_t seq_arr[127];
static bool seq_inited = false;

static void init_seq(void)
{
    seq_arr[0] = 0;
    seq_arr[1] = 0;
    seq_arr[2] = 0;
    seq_arr[3] = 0;
    seq_arr[4] = 0;
    seq_arr[5] = 0;
    seq_arr[6] = 1;                 /* first seeded value */

    for (idx_t i = 7; i < 127; ++i)
        seq_arr[i] = seq_arr[i - 1] ^ seq_arr[i - 7];

    seq_inited = true;
}

seq_t a(idx_t n)
{
    if (!seq_inited) init_seq();
    /* period is 127 */
    size_t idx = (size_t)(n % 127);
    return seq_arr[idx];
}
