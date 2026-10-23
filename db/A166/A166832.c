/*
 * A166832  Number of nX3 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(-1105344 + 219960*n + 516656*n^2 + 235676*n^3 *
 *            + 53879*n^4 + 8015*n^5 + 569*n^6 + 29*n^7)/181440
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - (seq_t)1;          /* n-1 */

    /* Horner's method to evaluate the 7th‑degree polynomial Q(n) */
    seq_t q = (seq_t)29;
    q = q * nn + (seq_t)569;
    q = q * nn + (seq_t)8015;
    q = q * nn + (seq_t)53879;
    q = q * nn + (seq_t)235676;
    q = q * nn + (seq_t)516656;
    q = q * nn + (seq_t)219960;
    q = q * nn + (seq_t)(-1105344);

    /* numerator = n*(n-1)*Q(n) */
    seq_t num = nn * n1;
    num = num * q;

    /* final division by 181440 (exact integer division) */
    seq_t res = num / (seq_t)181440;

    return res;
}
