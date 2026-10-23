/*
 * A137017  Numbers k such that k and k^2 use only the digits 1, 2, 6 and 9.
 * https://oeis.org/A137017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A137017: numbers k such that k and k² use only the digits 1, 2, 6, 9.
   The function generates, in increasing numeric order, all integers composed of
   these digits and keeps those whose square also contains only these digits. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *vals = NULL;          /* array of discovered terms   */
static size_t valcap = 0;           /* allocated capacity          */
static size_t valcnt = 0;           /* number of stored terms      */
static idx_t  cur_len = 0;          /* maximal length processed    */

static const seq_t digits[4] = {1, 2, 6, 9};

/* test whether all decimal digits of x are in {1,2,6,9} */
static bool allowed_digits(seq_t x) {
    if (x == 0) return false;      /* zero never allowed */
    while (x > 0) {
        int d = (int)(x % 10);
        if (d != 1 && d != 2 && d != 6 && d != 9) return false;
        x /= 10;
    }
    return true;
}

/* recursive generation of numbers with exactly 'len' digits,
   building current value 'cur' so far (starting at 0). */
static void gen_rec(idx_t len, idx_t pos, seq_t cur) {
    if (pos == len) {
        if (allowed_digits(cur) && allowed_digits(cur * cur)) {
            if (valcnt == valcap) {
                valcap = (valcap == 0) ? 64 : valcap * 2;
                vals = (seq_t *)realloc(vals, valcap * sizeof(seq_t));
            }
            vals[valcnt++] = cur;
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        seq_t next = cur * 10 + digits[i];
        gen_rec(len, pos + 1, next);
    }
}

/* ensure that we have at least 'need' terms stored */
static void generate_terms(idx_t need) {
    while (valcnt < need) {
        ++cur_len;
        gen_rec(cur_len, 0, 0);
    }
}

/* public API */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n = 0 */
    if (valcnt < n) generate_terms(n);
    return vals[n - 1];
}
