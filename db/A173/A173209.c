/*
 * A173209  Partial sums of A000069.
 * https://oeis.org/A173209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{k=1..n} A000069(k) -- the sum of the first n odious
 *            numbers (numbers with an odd -- number of 1‑bits in binary)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9321, u64 n <= 9321,
 *            u128 n <= 9321, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t  sum   = 0;          /* result */
    idx_t  count = 0;          /* how many odious numbers gathered  */
    idx_t  num   = 1;          /* candidate number            */

    while (count < n) {
        /* __builtin_parityll(x) returns 1 if x has odd popcount */
        if (__builtin_parityll((unsigned long long)num)) {
            sum += (seq_t)num;
            ++count;
        }
        ++num;
    }
    return sum;
}
