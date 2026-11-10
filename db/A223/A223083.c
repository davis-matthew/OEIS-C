/*
 * A223083  Trajectory of 64 under the map n-> A006369(n).
 * https://oeis.org/A223083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 64, a(n+1) = A006369(a(n)), where A006369(3q)=2q,
 *            A006369(3q+1)=4q+1, A006369(3q+2)=4q+3
 * Range:     u8 n <= 8, u16 n <= 87, u32 n <= 307, u64 n <= 861,
 *            u128 n <= 1714, big any n
 * Validated: standard, pass, 64 terms through n = 9842 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A006369_step(seq_t x)
{
    seq_t q = x / 3;
    seq_t r = x % 3;
    if (r == 0) {
        return q * 2;
    } else if (r == 1) {
        return q * 4 + 1;
    } else {
        return q * 4 + 3;
    }
}

seq_t a(idx_t n)
{
    seq_t x = 64;
    idx_t i;
    for (i = 1; i < n; i++) {
        x = A006369_step(x);
    }
    return x;
}
