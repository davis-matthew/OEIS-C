/*
 * A079664  a(n) = Sum_{i=1..n} LookAndSay(i).
 * https://oeis.org/A079664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum_{i=1..n} LookAndSay(i)
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the look‑and‑say value of a non‑negative
   integer i by reading its decimal digits from most significant
   to least significant and concatenating the run lengths with
   the corresponding digits.  Then it sums these values for
   all i between 1 and n inclusive.  The implementation uses
   only fixed‑size buffers and does not rely on any external
   helper functions. */

#include <stdint.h>

/* Helper: compute the look‑and‑say of a single integer x.
   The result fits easily into seq_t for the range of n of
   interest.  The function is independent of seq_t size. */
static seq_t look_and_say(idx_t x) {
    /* Buffer to hold decimal digits of x in reverse order. */
    int digits[32];
    int len = 0;

    /* Special case: if x is 0, treat it as one 0. */
    if (x == (idx_t)0) {
        digits[len++] = 0;
    } else {
        while (x != (idx_t)0) {
            digits[len++] = (int)(x % (idx_t)10);
            x /= (idx_t)10;
        }
    }

    /* Build the output number while iterating over digits
       from most significant to least.  The result is built
       incrementally by appending digits of counts followed
       by the digit itself. */
    seq_t res = 0;
    int run_len = 1;
    int prev_digit = digits[len - 1];

    for (int pos = len - 2; pos >= 0; --pos) {
        int d = digits[pos];
        if (d == prev_digit) {
            ++run_len;
        } else {
            /* flush previous run */
            int cnt = run_len;
            /* buffer digits of cnt in reverse order */
            int cnt_digits[20];
            int cd = 0;
            do {
                cnt_digits[cd++] = cnt % 10;
                cnt /= 10;
            } while (cnt > 0);
            for (int k = cd - 1; k >= 0; --k) {
                res = res * 10 + cnt_digits[k];
            }
            res = res * 10 + prev_digit;
            /* start new run */
            run_len = 1;
            prev_digit = d;
        }
    }

    /* flush final run */
    int cnt = run_len;
    int cnt_digits[20];
    int cd = 0;
    do {
        cnt_digits[cd++] = cnt % 10;
        cnt /= 10;
    } while (cnt > 0);
    for (int k = cd - 1; k >= 0; --k) {
        res = res * 10 + cnt_digits[k];
    }
    res = res * 10 + prev_digit;

    return res;
}

/* Main function: returns a(n) = Sum_{i=1..n} LookAndSay(i). */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    seq_t sum = 0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        sum += look_and_say(i);
    }
    return sum;
}
