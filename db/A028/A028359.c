/*
 * A028359  Two-bell analog of A028355.
 * https://oeis.org/A028359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the concatenation of the digits from the infinite
 *            repeating sequence 1,2,1,2,… whose digit sums are 1,2,3,4,…
 *            respectively. Let c_n be the index of the last digit used for
 *            a(n). Then sum_{i=1}^{c_n} digit_i = n(n+1)/2, where digit_i is
 *            1 for i odd and 2 for i even. a(n) consists of the di...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute c_n  */
    idx_t t = n * (n + 1) / 2;          /* triangular number */
    idx_t c_n;
    if (t % 3 == 0)
        c_n = 2 * t / 3;
    else                    /* t % 3 == 1 */
        c_n = (2 * t + 1) / 3;

    /* Compute c_{n-1}  */
    idx_t t_prev = (n - 1) * n / 2;
    idx_t c_prev;
    if (t_prev % 3 == 0)
        c_prev = 2 * t_prev / 3;
    else
        c_prev = (2 * t_prev + 1) / 3;

    /* Length and starting position of this block */
    idx_t len   = c_n - c_prev;
    idx_t start = c_prev + 1;

    seq_t digit  = (start & 1) ? (seq_t)1 : (seq_t)2;
    seq_t result = (seq_t)0;

    for (idx_t i = 0; i < len; ++i) {
        result = result * ((seq_t)10) + digit;
        digit  = (seq_t)(3 - digit);     /* toggle 1 ↔ 2 */
    }
    return result;
}
