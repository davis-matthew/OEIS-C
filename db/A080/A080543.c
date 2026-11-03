/*
 * A080543  In binary representation: keep the first digit and rotate left the
 *          others twice.
 * https://oeis.org/A080543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = keep the first binary digit and rotate the remaining bits
 *            left by 2 positions
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 88 terms through n = 8082 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a080541(idx_t n)
{
    /* n is at least 1 */
    if (n <= 3)                /* identity for 1‑3 */
        return (seq_t)n;

    /* number of bits in n */
    unsigned L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    unsigned k = L - 1;                     /* bits after the leading 1 */
    seq_t msb = ((seq_t)1) << (L - 1);       /* leading 1 */
    seq_t rem = (seq_t)n & (~msb);          /* remaining bits */

    /* rotate left by 2 within k bits */
    seq_t rotated = ((rem << 2) | (rem >> (k - 2))) & (((seq_t)1 << k) - 1);

    return msb | rotated;
}

seq_t a(idx_t n)
{
    return a080541(n);
}
