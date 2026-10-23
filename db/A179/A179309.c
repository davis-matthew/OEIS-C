/*
 * A179309  The smallest number that has more copies of some digit than any
 *          previous number in the sequence.
 * https://oeis.org/A179309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A179309: the nth smallest positive integer that has more copies of
   some digit than any previous integer in the sequence.  For n ≥ 1:
     let k = (n‑1)/10 + 1   (block number, number of repeated digits)
     let r = (n‑1)%10       (position within the block)
   If r < 9, a(n) = the digit (r+1) repeated k times;
   otherwise a(n) = 10^k. */
seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 10 + 1;
    idx_t r = (n - 1) % 10;

    if (r < 9) {
        seq_t val = 0;
        unsigned digit = r + 1;
        for (idx_t i = 0; i < k; ++i) {
            val = val * 10 + digit;
        }
        return val;
    } else { /* r == 9, digit 0 */
        seq_t val = 1;
        for (idx_t i = 0; i < k; ++i) {
            val *= 10;
        }
        return val;
    }
}
