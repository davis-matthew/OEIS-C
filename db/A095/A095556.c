/*
 * A095556  a(n) = 82 written in base n.
 * https://oeis.org/A095556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation of 82 in base n.
   For n=1 this is the unary expansion: 82 ones.
   For n>1 the base‑n representation of 82 is concatenated
   digit by digit; single‑digit values <10 contribute one
   decimal digit, two‑digit values contribute two decimal
   digits (e.g. 10→"10"). */
seq_t a(idx_t n)
{
    /* Handle base 1 separately (unary). */
    if (n == 1) {
        seq_t res = 0;
        /* 82 ones. */
        for (idx_t i = 0; i < 82; ++i)
            res = res * 10 + 1;
        return res;
    }

    const seq_t value = 82;
    seq_t base = (seq_t)n;
    seq_t digit;
    seq_t temp = value;

    /* Temporary storage for digits in reverse order. */
    seq_t digits[64];
    idx_t cnt = 0;

    /* Extract base‑n digits, least significant first. */
    while (temp > 0) {
        digit = temp % base;
        temp = temp / base;
        digits[cnt++] = digit;
    }

    /* Build the decimal concatenation of the digits,
       from most significant to least. */
    seq_t res = 0;
    for (idx_t i = cnt; i-- > 0;) {
        digit = digits[i];
        /* digit < 10 -> one decimal digit; otherwise two. */
        if (digit >= 10)
            res = res * 100 + digit;
        else
            res = res * 10 + digit;
    }

    return res;
}
