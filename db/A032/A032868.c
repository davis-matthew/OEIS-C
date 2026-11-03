/*
 * A032868  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑5 representation
 *            satisfies d(m) >= d(m-1) <= d(m-2) >= d(m-3) <= … (alternating
 *            inequalities)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether a value satisfies the alternating
   inequality property in its base‑5 digits. */
static bool valid_alt_base5(seq_t val)
{
    if (val == 0)          /* the sequence starts at 1 */
        return false;

    /* Store base‑5 digits (least significant first).  The sequence
       fits easily in less than 60 digits for 64‑bit types; 80 is safe
       for larger arbitrary‑precision types. */
    unsigned char d[80];
    int len = 0;
    seq_t tmp = val;
    while (tmp > 0) {
        d[len++] = (unsigned char)(tmp % 5);
        tmp /= 5;
    }

    /* Compare digits from the most significant side toward the least. */
    int m = len - 1;                     /* index of most significant digit */
    for (int i = m - 1; i >= 0; --i) {
        /* k = m - i   is the step number (1,2,3,...). */
        int k = m - i;
        if (k & 1) {           /* odd step: current >= next */
            if (d[i + 1] < d[i]) return false;
        } else {               /* even step: current <= next */
            if (d[i + 1] > d[i]) return false;
        }
    }
    return true;
}

/* Entry point for the sequence.  Position n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the given offset */

    idx_t count = 0;
    seq_t x = 1;
    for (;;) {
        if (valid_alt_base5(x)) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
