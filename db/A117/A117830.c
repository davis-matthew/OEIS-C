/*
 * A117830  Let S_m be the infinite sequence formed by starting with m and
 *          repeatedly reversing the digits and adding 4 to get the next term.
 *          For all m < 1015, S_m enters the cycle of length 54 whose terms
 *          are shown here.
 * https://oeis.org/A117830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117830: iterate reverse_digits(x)+4 starting from 5. */
/*  a(1) = 5, a(n) = reverse_digits(a(n-1)) + 4   for n ≥ 2. */

#include <stdbool.h>
#include "../utils/reverse_digits.h"

static seq_t  cycle[64];      /* storage for the 54‑term cycle */
static idx_t   cycle_len   = 0;
static bool    initialized = false;

/* Build the cycle once.  The first element of the cycle is 5.  */
static void init_cycle(void)
{
    if (initialized) return;

    seq_t cur = (seq_t)5;
    idx_t i   = 0;
    while (1) {
        cycle[i++] = cur;
        seq_t next = reverse_digits(cur) + (seq_t)4;
        if (next == cycle[0]) {          /* cycle closed */
            cycle_len = i;
            break;
        }
        cur = next;
    }
    initialized = true;
}

/* Return the n‑th term of the cycle (offset 1).  */
seq_t a(idx_t n)
{
    if (!initialized) init_cycle();
    /* a(1) corresponds to cycle[0] */
    idx_t idx = (n - 1) % cycle_len;
    return cycle[idx];
}
