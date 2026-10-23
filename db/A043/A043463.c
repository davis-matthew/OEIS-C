/*
 * A043463  Numbers having three 2's in base 9.
 * https://oeis.org/A043463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having three 2's in base 9. */

static inline unsigned long long count_twos(seq_t x)
{
    unsigned long long c = 0;
    const seq_t mod9 = (seq_t)9;
    const seq_t two  = (seq_t)2;
    while (x != (seq_t)0) {
        if ((x % mod9) == two) ++c;
        x /= mod9;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* offset is 1; safety guard */
    idx_t found   = 0;
    seq_t value   = (seq_t)0;
    for (;; ++value) {
        if (count_twos(value) == 3) {
            ++found;
            if (found == n) return value;
        }
    }
}
