/*
 * A169910  a(n) = n+n in carryless digital root arithmetic in base 10.
 * https://oeis.org/A169910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + n in carryless digital root arithmetic in base 10
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    unsigned digits[20];
    unsigned dcount = 0;
    idx_t t = n;
    while (t != 0) {
        unsigned d = (unsigned)(t % 10);
        t /= 10;
        unsigned prod = 2 * d;
        unsigned rem = prod % 9;
        unsigned newdig = (rem == 0) ? (d == 0 ? 0 : 9) : rem;
        digits[dcount++] = newdig;
    }
    for (int i = dcount; i-- > 0; ) {
        res = res * 10 + digits[i];
    }
    return res;
}
