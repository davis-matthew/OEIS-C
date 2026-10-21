/*
 * A098946  Numbers where 3 is the only odd decimal digit.
 * https://oeis.org/A098946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3798, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the set of positive integers whose decimal expansion
   consists solely of the digits {0,2,3,4,6,8} and contains at least one
   digit 3.  The terms are listed in increasing order.  The routine
   returns the nth term (1‑based).  It counts numbers of each length
   using the formula
      f(L) = 5*6^(L-1) – 4*5^(L-1),
   where the first factor counts all L‑digit strings with the first digit
   nonzero, and the second subtracts those that contain no 3 at all.  It
   then builds the nth number by a lexicographic choice of the digits. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper to supply powers of 6 and 5 in arbitrary‑precision seq_t. */
#define MAXLEN 64
static seq_t pow6[MAXLEN];
static seq_t pow5[MAXLEN];
static int pow6_len = 0;
static int pow5_len = 0;
static bool pow_initialized = false;

static void ensure_powers(void) {
    if (pow_initialized) return;
    /* initialise first powers */
    pow6[0] = pow5[0] = (seq_t)1;
    pow6_len = 1;
    pow5_len = 1;
    pow_initialized = true;
}

static void ensure_pow6(int e) {
    while (pow6_len <= e) {
        pow6[pow6_len] = pow6[pow6_len - 1] * (seq_t)6;
        pow6_len++;
    }
}

static void ensure_pow5(int e) {
    while (pow5_len <= e) {
        pow5[pow5_len] = pow5[pow5_len - 1] * (seq_t)5;
        pow5_len++;
    }
}

/* The six allowed digits in increasing order. */
static const int DIGITS[6] = {0, 2, 3, 4, 6, 8};

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* index 0 is undefined */
    seq_t remaining = (seq_t)n;           /* work in seq_t for safety */

    ensure_powers();

    /* Find the length of the n-th term. */
    int len = 1;
    for (;;) {
        ensure_pow6(len - 1);
        ensure_pow5(len - 1);

        seq_t count_len = (seq_t)5 * pow6[len - 1] -
                          (seq_t)4 * pow5[len - 1];
        if (remaining <= count_len) break;
        remaining -= count_len;
        len++;
    }

    /* Construct the number digit by digit. */
    bool has3 = false;
    seq_t value = (seq_t)0;

    for (int pos = 0; pos < len; ++pos) {
        for (int i = 0; i < 6; ++i) {
            int d = DIGITS[i];
            if (pos == 0 && d == 0) continue;   /* no leading zero */

            bool next_has3 = has3 || (d == 3);
            int rem_pos = len - pos - 1;

            ensure_pow6(rem_pos);
            ensure_pow5(rem_pos);

            seq_t cnt;
            if (next_has3)
                cnt = pow6[rem_pos];                     /* all completions valid */
            else
                cnt = pow6[rem_pos] - pow5[rem_pos];      /* exclude those without 3 */

            if (cnt < remaining) {
                remaining -= cnt;   /* skip this block */
            } else {
                /* This digit is part of the answer. */
                value = value * (seq_t)10 + (seq_t)d;
                has3 = next_has3;
                break;
            }
        }
    }

    return value;
}
