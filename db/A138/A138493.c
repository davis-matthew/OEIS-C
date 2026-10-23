/*
 * A138493  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 9.
 * https://oeis.org/A138493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0) = 9; For n > 0 let a(n) be formed from a(n‑1) by • reading
 *            its decimal digits from right to left, • counting how many times
 *            each digit occurs, • outputting for each digit (in order of
 *            first appearance) the count followed by the digit
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the next term in the sequence from a given term. */
static seq_t next_term(seq_t cur)
{
    unsigned char seen[10] = {0};
    unsigned char count[10] = {0};
    unsigned char order[10];
    int order_len = 0;

    seq_t x = cur;
    if (x == 0) {               /* not expected, but handled */
        order[0] = 0;
        order_len = 1;
        count[0] = 1;
    } else {
        while (x != 0) {
            unsigned d = (unsigned)(x % 10);
            if (!seen[d]) {
                seen[d] = 1;
                order[order_len++] = d;
            }
            count[d]++;
            x /= 10;
        }
    }

    unsigned short out_digits[64];
    int out_len = 0;

    for (int i = 0; i < order_len; ++i) {
        unsigned d = order[i];
        unsigned int cnt = count[d];

        /* convert cnt to decimal digits */
        unsigned short tmp[10];
        int tlen = 0;
        unsigned int val = cnt;
        if (val == 0) {
            tmp[tlen++] = 0;
        } else {
            while (val != 0) {
                tmp[tlen++] = val % 10;
                val /= 10;
            }
        }
        for (int k = tlen - 1; k >= 0; --k) {
            out_digits[out_len++] = tmp[k];
        }
        out_digits[out_len++] = d;
    }

    seq_t next = 0;
    for (int i = 0; i < out_len; ++i) {
        next = next * 10 + out_digits[i];
    }
    return next;
}

seq_t a(idx_t n)
{
    if (n == 0) return 9;

    seq_t cur = 9;
    for (idx_t i = 1; i <= n; ++i) {
        cur = next_term(cur);
    }
    return cur;
}
