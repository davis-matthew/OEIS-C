/*
 * A115266  Row sums of correlation triangle for floor((n+3)/3).
 * https://oeis.org/A115266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n^4 + 40*n^3 + 276*n^2 + 792*n + 837 + 27*(-1)^n -
 *            32*(n+5)*(n mod 3))/864
 * Range:     u8 none, u16 n <= 8, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t expr = 2*nn*nn*nn*nn + 40*nn*nn*nn + 276*nn*nn + 792*nn + 837;
    if ((n & 1) == 0)
        expr += 27;
    else
        expr -= 27;
    seq_t subtract = 32 * (nn + 5) * (n % 3);
    expr -= subtract;
    return expr / 864;
}
