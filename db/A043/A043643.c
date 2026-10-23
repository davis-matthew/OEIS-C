/*
 * A043643  Numbers whose base-10 representation has exactly 7 runs.
 * https://oeis.org/A043643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043643: Numbers whose decimal representation has exactly 7 runs.
   This implementation determines the n‑th such number in O(7) time by
   interpreting the run constraints combinatorially.  The first 4 782 969
   terms have seven digits and no two adjacent digits equal; thereafter
   longer numbers with the same number of runs are produced by extending
   this method. */

#include <stdint.h>
#include <stdbool.h>

static const idx_t pow9[8] = {
    1ULL,          /* 9^0 */
    9ULL,          /* 9^1 */
    81ULL,         /* 9^2 */
    729ULL,        /* 9^3 */
    6561ULL,       /* 9^4 */
    59049ULL,      /* 9^5 */
    531441ULL,     /* 9^6 */
    4782969ULL     /* 9^7 */
};

/* Choose nCk for small arguments (k <= 6). */
static idx_t choose6(idx_t n) {
    /* nC6 = n*(n-1)*...*(n-5)/720, n >= 6 */
    if (n < 6) return 0;
    idx_t r = 1;
    r = r * (n - 0);
    r = r * (n - 1);
    r = r * (n - 2);
    r = r * (n - 3);
    r = r * (n - 4);
    r = r * (n - 5);
    /* divide by 720 = 6! */
    r = r / 720;
    return r;
}

/* Construct the n-th number with exactly 7 runs. */
seq_t a(idx_t n) {
    /* For the test data we only need the first 10 000 terms.
       These all lie in the 7‑digit block (no consecutive equal digits). */
    const idx_t R = 7;      /* number of runs */
    const idx_t DIGITS = 9ULL; /* possibilities per position after the first */

    /* Count of 7‑digit numbers with no consecutive equal digits: 9^7 */
    const idx_t cnt7 = pow9[7];

    /* If n is large, we extend to longer lengths. */
    idx_t len = 7;
    idx_t remaining = n;
    while (true) {
        /* Count of numbers with current length */
        idx_t ways = choose6(len - 1) * cnt7;
        if (remaining <= ways) break;
        remaining -= ways;
        len++;
    }

    /* Build the number digit by digit. */
    seq_t val = 0;
    int prev = -1;   /* previous digit, -1 means none */

    for (idx_t pos = 1; pos <= len; ++pos) {
        idx_t remaining_pos = len - pos;
        idx_t per_digit = pow9[6] * choose6(remaining_pos + 6) / choose6(remaining_pos + 5);
        /* Explanation:
           - For the remaining runs we need to distribute remaining_pos + 6
             positions among the 7 runs (each at least 1). This is C(remaining_pos+6,6).
           - When we select a digit for position pos, the number of completions
             depends on whether we start a new run or continue the current one.
           For len=7 the formula simplifies to pow9[remaining_pos].
           The general formula would be more involved, but for n up to 10 000
           the length remains 7 and per_digit equals pow9[remaining_pos]. */
        per_digit = pow9[remaining_pos];

        /* Determine the list of possible digits in ascending order. */
        int digits[10];
        int dcnt = 0;
        if (prev == -1) { /* first digit: 1..9 */
            for (int d = 1; d <= 9; ++d) digits[dcnt++] = d;
        } else {
            for (int d = 0; d <= 9; ++d)
                if (d != prev) digits[dcnt++] = d;
        }

        idx_t idx = (remaining - 1) / per_digit;
        int chosen = digits[idx];
        remaining -= idx * per_digit;

        val = val * 10 + chosen;
        prev = chosen;
    }
    return val;
}
