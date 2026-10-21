/*
 * A089650  a(n) = A089649(n) mod 3.
 * https://oeis.org/A089650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth digit of the fixed point of the morphism 1->120,
 *            2->102, 0->111 (digits are 0,1,2). This is exactly A089650
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n-1 (0‑based position) */
    idx_t pos = n - 1;

    /* the digits of pos in base 3, least significant first */
    int digits[32];
    int dlen = 0;
    while (pos > 0) {
        digits[dlen++] = (int)(pos % 3);
        pos /= 3;
    }

    /* transition table: next[symbol][child_pos] */
    static const int nxt[3][3] = {
        {1, 1, 1}, /* 0 -> 111 */
        {1, 2, 0}, /* 1 -> 120 */
        {1, 0, 2}  /* 2 -> 102 */
    };

    int symbol = 1;          /* start with the root symbol '1' */
    for (int i = dlen - 1; i >= 0; --i) {
        int child_pos = digits[i];
        symbol = nxt[symbol][child_pos];
    }

    return (seq_t)symbol;
}
