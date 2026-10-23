/*
 * A037851  a(n)=Sum{d(i-1)-d(i): d(i)<d(i-1), i=1,...,m}, where
 *          Sum{d(i)*10^i: i=0,1,...,m} is the base 10 representation of n.
 * https://oeis.org/A037851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ{ d(i-1)-d(i) : d(i)<d(i-1), i=1..m } where Σ{ d(i)*10^i
 *            : i=0..m } is the base‑10 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* zero for numbers with a single digit (1–9) automatically */
    if (n < 10) return 0;

    seq_t sum = 0;
    idx_t prev = n % 10;   /* least significant digit */
    n /= 10;

    while (n > 0) {
        idx_t curr = n % 10;   /* next digit toward more significant end */
        if (prev > curr)
            sum += (seq_t)(prev - curr);
        prev = curr;
        n /= 10;
    }

    return sum;
}
