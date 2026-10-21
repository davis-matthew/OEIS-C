/*
 * A095506  a(n) = 57 written in base n.
 * https://oeis.org/A095506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = decimal value of 57 written in base‑n (base 1 is unary) */

seq_t a(idx_t n)
{
    /* Special handling for the unary base */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 57; ++i)
            res = res * 10 + 1;   /* 57 ones */
        return res;
    }

    /* Value to represent in base n */
    const seq_t value = 57;
    const seq_t base  = (seq_t)n;

    /* Extract digits of value in base n (least significant first) */
    seq_t digits[16];          /* 57 in base 2 needs at most 6 digits, base 3 at most 5, etc. */
    idx_t dcnt = 0;
    seq_t tmp = value;
    while (tmp > 0) {
        digits[dcnt++] = tmp % base;
        tmp /= base;
    }
    if (dcnt == 0)            /* only possible for value == 0, not here */
        digits[dcnt++] = 0;

    /* Build the decimal number by concatenating the decimal representations
       of the digits, most significant digit first.  */
    seq_t result = 0;
    for (idx_t i = dcnt; i > 0; ) {
        --i;                  /* i now indexes the most significant digit first */
        seq_t d = digits[i];

        /* Determine decimal length of d */
        unsigned len = 1;
        seq_t tmp_d = d;
        while (tmp_d >= 10) {
            tmp_d /= 10;
            ++len;
        }

        /* 10^len */
        seq_t pow10 = 1;
        for (unsigned j = 0; j < len; ++j)
            pow10 *= 10;

        result = result * pow10 + d;
    }

    return result;
}
