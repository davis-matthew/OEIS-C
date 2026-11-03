/*
 * A039481  Numbers whose base-11 representation has the same nonzero number
 *          of 5's and 9's.
 * https://oeis.org/A039481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int equal_fives_nines(seq_t v)
{
    unsigned int count5 = 0, count9 = 0;
    seq_t x = v;
    while (x) {
        unsigned int d = (unsigned int)(x % 11);
        if (d == 5) ++count5;
        else if (d == 9) ++count9;
        x = x / 11;
    }
    return (count5 == count9 && count5 > 0);
}
/* Sequence A039481: Numbers whose base‑11 representation has the same
   nonzero number of 5's and 9's.  */

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;
    while (count < n) {
        if (equal_fives_nines(val)) {
            ++count;
        }
        val = val + 1;
    }
    return val - 1;
}
