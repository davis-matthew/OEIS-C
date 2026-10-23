/*
 * A118527  Start with 1 and repeatedly reverse the digits and add 8 to get
 *          the next term.
 * https://oeis.org/A118527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1, a(n)=rev_decimal(a(n-1))+8 where rev_decimal
 *            reverses the decimal digits of a number
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool cycle_init = false;
static seq_t cycle[27];

/* Reverse the decimal digits of a value of type seq_t. */
static seq_t reverse_decimal_local(seq_t v) {
    seq_t rev = (seq_t)0;
    while (v > (seq_t)0) {
        seq_t digit = v % (seq_t)10;
        rev = rev * (seq_t)10 + digit;
        v = v / (seq_t)10;
    }
    return rev;
}

/* Build the 27‑term cycle starting with the value 9. */
static void init_cycle(void) {
    seq_t x = (seq_t)9;
    for (idx_t i = 0; i < 27; ++i) {
        cycle[i] = x;
        x = reverse_decimal_local(x) + (seq_t)8;
    }
    cycle_init = true;
}

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    if (!cycle_init) {
        init_cycle();
    }
    idx_t pos = (n - (idx_t)2) % (idx_t)27;
    return cycle[pos];
}
