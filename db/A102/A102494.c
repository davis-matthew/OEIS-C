/*
 * A102494  Numbers in base-60 representation that cannot be written with
 *          decimal digits.
 * https://oeis.org/A102494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 206, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102494: a(n) is the n‑th positive integer whose base‑60
   representation contains at least one digit ≥ 10. */

static int has_non_decimal(idx_t x)
{
    while (x) {
        if (x % 60 >= 10)
            return 1;
        x /= 60;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        if (has_non_decimal(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
