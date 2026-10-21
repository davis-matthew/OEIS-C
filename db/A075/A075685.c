/*
 * A075685  Reverse and Add! carried out in base 4; number of steps needed to
 *          reach a palindrome, or -1 if no palindrome is ever reached.
 * https://oeis.org/A075685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse‑and‑Add sequence A075685 in base‑4.
   Returns the number of reverse+add steps required to obtain a
   base‑4 palindrome; returns (seq_t)-1 if no palindrome is ever
   reached.  Uses Floyd’s cycle detection to avoid unbounded memory. */

#include <stdbool.h>

/* Reverse a seq_t number in base 4. */
static seq_t rev_base4(seq_t x)
{
    seq_t rev = 0;
    while (x != 0) {
        rev = rev * 4 + (x % 4);
        x   = x / 4;
    }
    return rev;
}

/* Test whether a seq_t number is a palindrome in base 4. */
static bool pal_base4(seq_t x)
{
    return rev_base4(x) == x;
}

/* Step: current -> current + reverse(current) (base‑4). */
static seq_t next_step(seq_t cur)
{
    return cur + rev_base4(cur);
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;

    /* Immediate palindrome? */
    if (pal_base4(cur))
        return (seq_t)0;

    /* Initialise tortoise–hare pointers. */
    seq_t tort = cur;                    /* one step performed so far */
    seq_t hare = next_step(cur);         /* two steps ahead of start */

    seq_t steps = 0;

    while (true) {
        /* Check for palindrome on the current tortoise position. */
        if (pal_base4(tort))
            return steps;

        /* Detect cycle: if tortoise and hare meet, a loop exists. */
        if (tort == hare)
            return (seq_t)-1;

        /* Advance pointers. */
        seq_t next_tort = next_step(tort);     /* tortoise moves one step */
        seq_t next_hare = next_step(hare);     /* hare moves two steps in total */
        next_hare      = next_step(next_hare);

        tort = next_tort;
        hare = next_hare;
        ++steps;
    }
}
