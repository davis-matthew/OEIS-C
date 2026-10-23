/*
 * A033897  Sort then Add, a(1)=13.
 * https://oeis.org/A033897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=13; a(n)=a(n-1)+sort_asc(a(n-1)) */
/* sort_asc(x) is the integer formed by sorting the decimal digits of x in ascending order */

static seq_t sorted_digits(seq_t x)
{
    int count[10] = {0};
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        ++count[d];
        x = x / (seq_t)10;
    }
    seq_t y = (seq_t)0;
    for (int d = 0; d <= 9; ++d) {
        int c = count[d];
        while (c--) {
            y = y * (seq_t)10 + (seq_t)d;
        }
    }
    return y;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* behaviour below offset is irrelevant */
    }

    seq_t current = (seq_t)13;   /* a(1) */

    if (n == 1) {
        return current;
    }

    for (idx_t i = 2; i <= n; ++i) {
        seq_t add = sorted_digits(current);
        current = current + add;
    }

    return current;
}
